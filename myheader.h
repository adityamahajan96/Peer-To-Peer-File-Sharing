#ifndef MYHEADER_H
#define MYHEADER_H

#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<fstream>
#include<pthread.h>
//#include<openssl/sha.h.>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>

#define MAX_CLIENTS 10
#define CHUNK_SIZE

using namespace std;

class Client {
public:
	   string port;
	   string ip;
	   string user_id;
	   string password;
};

class Group {
public:
	string groupid;
	Client owner;
	vector<Client> clients;
};


#endif
