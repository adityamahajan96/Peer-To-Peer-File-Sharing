#include "myheader.h"
#include <bits/stdc++.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

using namespace std;

Client clients[MAX_CLIENTS];
vector<Group> groups;

void *serveClients(void *socket);

int main(int argc, char **argv) {
 
	string tracker_file, tracker_info, tracker_ip, tracker_port, info;
	int tport;
	tracker_file = argv[1];
	string tracker_no = argv[2];
	ifstream readtracker(tracker_file);
	while (getline (readtracker, info)) {
		tracker_info = info;
	}
	readtracker.close();
	
	for(int i=0;i<tracker_info.length();i++) {
		if(tracker_info.at(i) == ' ') continue;
		if(tracker_info.at(i) == ':') break;
		tracker_ip+=tracker_info.at(i);
	}
	stringstream tracker_str(tracker_info.substr(tracker_info.find(":") + 1)); 
	tracker_str >> tport;
	int server_fd, new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address);
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		cerr << "ERROR: Failed to create socket\n"; 
		return -1; 
	} 
	
	// Forcefully attaching socket to the port 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		cerr << "ERROR: Failed while trying to attach socket to port\n";
		return -2;
	} 
	address.sin_family = AF_INET; 
	address.sin_port = htons(tport);
	
	if (inet_pton(AF_INET, tracker_ip.c_str(), &address.sin_addr) <= 0) {
        	cerr << "\nInvalid address/ Address not supported \n";
        	return -3;
    	}
	
	// Forcefully attaching socket to the port 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		cerr << "ERROR: Failed while trying to bind\n"; 
		return -4;
	} 
	if (listen(server_fd, MAX_CLIENTS) < 0) { 
		cerr << "ERROR: Failed while trying to listen\n"; 
		return -5;
	}
	
	cout << "Listening at "<<tracker_ip<<":"<<tport<<endl;
	
	pthread_t newClient;	
	
	while(true) {
		
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
			cerr << "ERROR: Failed while trying to accept connections\n";
			return -3;
		} 
		
		
		int *arg = (int *)malloc(sizeof(*arg));
        	*arg = new_socket;
        	pthread_create(&newClient, 0, serveClients, arg);
	}

	return 0; 
}

void *serveClients(void *socket) {
	int new_socket = *((int *)socket);
	int clientCounter = 0, valread;
	Client curr_loggedin;
	curr_loggedin.user_id = "0000";
	char *send_buf;

	while(true) {
		char buffer[1024] = {0};
		valread = read(new_socket , buffer, 1024);

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
			if(clientCounter > MAX_CLIENTS) {
				cerr << "ERROR: User limit exceeded!\n";
				continue;
			}
			clients[clientCounter].user_id = details[1];
			clients[clientCounter].password = details[2];
			clients[clientCounter].ip = details[3];
			clients[clientCounter].port = details[4];
			
			string replyMsg = "User "+details[1]+" created";
			cout << replyMsg << endl;
			send_buf = new char[replyMsg.length() + 1];
            		strcpy(send_buf, replyMsg.c_str());

			send(new_socket , send_buf , strlen(send_buf) , 0);
			clientCounter++;
		}
		
		else if (msg_id == "login") {
			bool flag = false;
			for(int i=0;i<MAX_CLIENTS;i++) {
				if(clients[i].user_id == details[1]) {
					if(clients[i].password == details[2]) {
						cout << "Login successful" << endl;
						flag = true;
						curr_loggedin = clients[i];
						break;
					}
					else {
						cout << "Wrong Password!" << endl;
						flag = true;
						break;
					}
				}
			}
			
			if(!flag) {
				cerr << "ERROR: Cannot login. User doesn't exist.\n";
			}
		}
		
		else if (msg_id == "create_group") {
			if(curr_loggedin.user_id == "0000") {
				cerr << "ERROR: User not currently logged in\n";
			}
			else {
				string groupid = details[1];
				Group group;
				group.groupid = groupid;
				group.owner = curr_loggedin;
				group.clients.push_back(curr_loggedin);
				groups.push_back(group);
				cout << "Group created with group id : " << groupid << endl;
			}
			
		}
		
		else if(msg_id == "join_group")  {
			Client owner;
			for(auto &i : groups) {
				if(i.groupid == details[1]) {
					owner = i.owner;
					break;
				}
			}
			
			cout << "Sent join request to owner: "<<owner.user_id<<endl;
			string replyMsg = "User "+details[2]+":"+details[3]+" requested to join group "+details[1];

			send_buf = new char[replyMsg.length() + 1];
            		strcpy(send_buf, replyMsg.c_str());

			send(new_socket , send_buf , strlen(send_buf) , 0);

		}
		
		else if(msg_id == "leave_group") {
			
		}
		
		else if(msg_id == "list_groups") {
			string replyMsg = "";
			for(auto &i : groups) {
				cout << i.groupid<<endl;
				replyMsg+=i.groupid+"\n";
			}
			
			send_buf = new char[replyMsg.length() + 1];
            		strcpy(send_buf, replyMsg.c_str());

			send(new_socket , send_buf , strlen(send_buf) , 0);
		}
		
		else if(msg_id == "list_files") {
			
		}
		
		else if(msg_id == "upload_file") {
			
		}
		
		else if(msg_id == "download_file") {
		
		}
		
		//printf("%s\n",buffer);

	}

	return socket;
}
