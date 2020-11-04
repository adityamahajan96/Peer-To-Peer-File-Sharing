#include "myheader.h"

#define PORT 8080
#define IP "127.0.0.1"

using namespace std;

int create_or_login_acc(string user_id, string password, string msg_id) {
	
	int port = 8080;
	char* ip = "127.0.0.1";
	string message = msg_id+":"+user_id+":"+password+":"+IP+":8080"/*+port*/;
	//Create socket for message passing
	int sock_fd, e;
	//char buf[message.size()+1];
	char buf[1024];
    	strcpy(buf,message.c_str());
	struct sockaddr_in server_addr;
	char rec_buf[1024] = {0};
	
	/*if(sock_fd = socket(AF_INET, SOCK_STREAM, 0) == -1) {
		cerr << "ERROR: Unable to create socket\n";
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	
	if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
	
	e = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(e == -1) {
		cerr << "ERROR: Cannot connect to server\n";
		return -2;
	}*/
	
	int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    
    printf("%s\n",buf); 
	
	send(sock, buf, strlen(buf), 0);
	
	cout<<"Peer details sent for msg_id "<<msg_id<<endl;
	
	int rcvd_msg = recv(sock, rec_buf, strlen(rec_buf), 0);
	
	/*if(rcvd_msg == "err") {
		cerr << "ERROR: Unable to create account\n";
		return -3;
	}*/
	//close(sock);
	return 0;
}

int group_ops(string groupid, string msg_id) {

	int port = 8080;
	char* ip = "127.0.0.1";
	string message = msg_id+":"+groupid+":"+IP+":8080"/*+port*/;
	//Create socket for message passing
	int sock_fd, e;
	//char buf[message.size()+1];
	char buf[1024];
    	strcpy(buf,message.c_str());
	struct sockaddr_in server_addr;
	char rec_buf[1024] = {0};
		
	int sock = 0, valread; 
    	struct sockaddr_in serv_addr; 
    	char *hello = "Hello from client"; 
    	char buffer[1024] = {0}; 
    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Socket creation error \n"); 
		return -1; 
    	}
   
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(PORT); 
       
    	// Convert IPv4 and IPv6 addresses from text to binary form 
    	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  { 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
    	} 
   
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	} 
    
    	printf("%s\n",buf); 
	
	send(sock, buf, strlen(buf), 0);
	
	cout<<"Peer details sent for msg_id "<<msg_id<<endl;
	
	int rcvd_msg = recv(sock, rec_buf, strlen(rec_buf), 0);
	
	/*if(rcvd_msg == "err") {
		cerr << "ERROR: Unable to create account\n";
		return -3;
	}*/
	//close(sock);
	return 0;
}

int main(int argc, char **argv) {
	
	//OPTIONAL: Precheck if arguments are valid
	
	//Create Socket
	/*int sock_fd, e;
	int port = 8080;
	char* ip = "127.0.0.1";
	struct sockaddr_in server_addr;
	if(sock_fd = socket(AF_INET, SOCK_STREAM, 0) == -1) {
		cerr << "ERROR: Unable to create socket\n";
		return -1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	
	 if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
	
	//Connect to server
	
	e = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	if(e == -1) {
		cerr << "ERROR: Cannot connect to server\n";
		return -2;
	}
	*/
	
	/*int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    */
    //return 0;
	//Send info to tracker
	
	//Client stays alive
	
	while(true)  {
		string command_str;
		vector<string> vec_command;
		getline(cin, command_str);
		string temp = "";
		for(int i=0;i<command_str.length();i++) {
			if(i==command_str.length()-1) temp+=command_str.at(i);
			if(command_str.at(i) == ' ' || i == command_str.length()-1) {
				vec_command.push_back(temp);
				temp=""; 
				continue;
			}
			temp+=command_str.at(i);
		}
	
		if(vec_command[0] == "create_user") {
			string user_id = vec_command[1];
			string password = vec_command[2];
			if(create_or_login_acc(user_id, password, "create_user") < 0) {
				cerr << "ERROR: Some error occurred while creating user.\n";
			}
		}
		
		else if(vec_command[0] == "login") {
			string user_id = vec_command[1];
			string password = vec_command[2];
			if(create_or_login_acc(user_id, password, "login") < 0) {
				cerr << "ERROR: Some error occurred while logging in.\n";
			}
		}
		
		else if(vec_command[0] == "create_group") {
			string groupid = vec_command[1];
			group_ops(groupid, "create_group");
		}
		
		else if(vec_command[0] == "join_group") {
			string groupid = vec_command[1];
			group_ops(groupid, "join_group");
		}
		
		else if(vec_command[0] == "leave_group") {
			string groupid = vec_command[1];
			group_ops(groupid, "leave_group");
		}
		
		if(vec_command[0] == "create_user") {
			
		}
	
	}
}
