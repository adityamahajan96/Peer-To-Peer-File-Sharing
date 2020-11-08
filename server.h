#ifndef SERVER_H
#define SERVER_H
#include "myheader.h"

void *serveNewClients(void *socket);

void *client_as_server(void * client_info)  {
	int server_fd;
    	struct sockaddr_in address;
    	int opt = 1, new_socket;
    	int addrlen = sizeof(address);
    	string server_info = *(string *)client_info;
    	string server_ip = "";
    	for(int i=0;i<server_info.length();i++) {
    		if(server_info.at(i) == ' ') continue;
		if(server_info.at(i) == ':') break;
		server_ip+=server_info.at(i);
    	}
	string server_port = server_info.substr(server_info.find(":") + 1);
    	char *s_ip;
    	s_ip = new char[server_ip.length()];
    	strcpy(s_ip, server_ip.c_str());

    	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		cerr << "ERROR: Failed to create socket\n"; 
		return client_info; 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		cerr << "ERROR: Failed while trying to attach socket to port\n";
		return client_info;
	}
	
    	address.sin_family = AF_INET; 
	//address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(stoi(server_port));
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
    	if(inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr)<=0)  { 
        	printf("\nInvalid address/ Address not supported \n"); 
        	return client_info; 
    	} 

    	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		cerr << "ERROR: Failed while trying to bind\n"; 
		return client_info;
	} 
	if (listen(server_fd, MAX_CLIENTS) < 0) { 
		cerr << "ERROR: Failed while trying to listen\n"; 
		return client_info;
	} 
    	
    	pthread_t clientName;

    	while (true) {
    	
        	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
			cerr << "ERROR: Failed while trying to accept connections\n";
			return client_info;
		} 

		// Create new thread for each client and pass socket number as a argument.
		int *arg = (int *)malloc(sizeof(*arg));
		*arg = new_socket;
		pthread_create(&clientName, 0, serveNewClients, arg);
        //close(new_socket);
    	}
    
}

void *serveNewClients(void *socket) {
	/*int new_socket = *((int *)socket);
    	char buffer[1024] = {0};
    	read(new_socket, buffer, 1024);
    	string fname = string(buffer);
    	ifstream readInputFile(fname, ifstream::binary);
    	long int fsize, flag = 0, chksz;
    	struct stat statObj;
    	stat(fname.c_str(), &statObj);
    	fsize = statObj.st_size; // Size of a file.
    	chksz = CHUNK_SIZE;
    //readInputFile.seekg(0, ios::beg);
    long int NC = fsize / chksz; // Calculate number of chunks.
    if (NC == 0)
    {
        NC = 1;
        chksz = fsize;
    }
    cout << "server sending file to client" << endl;
    char *Buff;

    //  Send file from server to client.
    while (NC > 0)
    {
        Buff = new char[chksz];
        readInputFile.read(Buff, chksz);
        send(new_socket, Buff, chksz, 0);
        //logprinter(to_string(NC));
        NC--;
        if (NC == 0 && flag == 0)
        {
            flag = 1;
            long int t = fsize % chksz;
            if (t != 0)
            {
                NC = 1;
                chksz = t;
            }
        }
    }
    close(new_socket);
    readInputFile.close();
    cout << "file sent successfully -- server" << endl;*/
    	int new_socket = *((int *)socket);
	int clientCounter = 0, valread;
	Client curr_loggedin;
	curr_loggedin.user_id = "0000";
	char *send_buf;

	while(true) {
		char buffer[1024] = {0};
		valread = read(new_socket , buffer, 1024);
		cout << buffer<<endl;
	}

    return socket;
}

#endif
