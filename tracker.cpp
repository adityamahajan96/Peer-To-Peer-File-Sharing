/*#include "myheader.h"

#define PORT 9900

using namespace std;

map<int,int> sock2idx;
vector<int> sock_n(10,0);


int main () {
	
}*/

#include "myheader.h"
#include <bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
#define MAX_CLIENTS 10

using namespace std;

Client clients[MAX_CLIENTS];
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		cerr << "ERROR: Failed while trying to bind\n"; 
		return -1;
	} 
	if (listen(server_fd, MAX_CLIENTS) < 0) 
	{ 
		cerr << "ERROR: Failed while trying to listen\n"; 
		return -2;
	} 
	
	int clientCounter = 0;
	while(true) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
			cerr << "ERROR: Failed while trying to accept connections\n";
			return -3;
		} 
		
		valread = recv(new_socket , buffer, 1024, 0);
		char delim[2] = ":";
		char* temp = strtok(buffer, delim);
		
		//cout << temp << endl;
		string details[5];
		int i = 0 ;
		while(temp!=NULL) {
		
			details[i++] = temp;
			temp = strtok(NULL, delim);
		}
		
		string msg_id = details[0];
		
		if (msg_id == "create_user") {
			clients[clientCounter].user_id = details[1];
			clients[clientCounter].password = details[2];
			clients[clientCounter].ip = details[3];
			clients[clientCounter].port = details[4];
		}
		
		else if (msg_id == "login") {
			bool flag = false;
			for(int i=0;i<MAX_CLIENTS;i++) {
				if(clients[i].user_id == details[1]) {
					cout << "Login successful" << endl;
					break;
				}
			}
			
			if(!flag) {
				cout << "ERROR: Cannot login. User doesn't exist." <<endl;
			}
		}
		
		else if (msg_id == "create_group") {
		
		}
		//printf("%s\n",buffer);
		send(new_socket , hello , strlen(hello) , 0 ); 
		printf("Hello message sent\n"); 
		clientCounter++;
	}
	return 0; 
} 

