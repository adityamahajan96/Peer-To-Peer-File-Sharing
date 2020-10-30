#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
 
using namespace std;
 
int main() {

	// create socket
	int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(listening_socket == -1) {
		cerr << "ERROR: Unable to create a socket"<<endl;
		return -1;
	}
	
	// bind the socket to a port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(2400);
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
	
	if(bind(listening_socket, (sockaddr *)&hint, sizeof(hint) == -1)) {
		
		cerr << "ERROR: Unable to bind to IP/port"<<endl;
		return -2;
	}
	
	// mark the socket for listening in
	if(listen(listening_socket,SOMAXCONN) == -1) {
		cerr << "ERROR: Unable to listen"<<endl;
		return -3;
	}
	
	// accept a call
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];
	char svc[NI_MAXSERV];
	
	int clientSocket = accept(listening_socket, (sockaddr *)&client, &clientSize);
	
	if(clientSocket == -1) {
	
		cerr << "ERROR: Cannot connect with client"<<endl;
		return -4;
	
	}
	
	// close the listening socket
	close (listening_socket);
	memset(host, 0, NI_MAXHOST);
	memset(svc, 0, NI_MAXSERV);
	
	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
	
	if(result) {
		cout << host << " connected on "<<svc<<endl;
	}
	
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on "<<ntohs(client.sin_port) << endl;
	}
	
	// while receiving - display message, echo message
	char buf[4096];
	while(true) {
		// Clear the buffer
		memset(buf, 0, 4096);
		
		// Wait for a message
		int bytesRecv = recv(clientSocket, buf, 4096, 0);
		
		if(bytesRecv == -1) {
			cerr << "ERROR: There was a connection issue." <<endl;
			break;
		}
		
		if(bytesRecv == 0) {
			cout << "The client disconnected" <<endl;
			break;
		}
		
		// Display messaage
		cout << "Received: "<<string(buf, 0, bytesRecv) << endl;
		
		// Resend message
		send(clientSocket, buf, bytesRecv + 1, 0);
	}
	
	// close socket
	close(clientSocket);
	
	return 0;

}
