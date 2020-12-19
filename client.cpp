#include "myheader.h"
#include "server.h"

using namespace std;


int send_info_to(string user_id, string password, string msg_id) {
	
	
	/*if(rcvd_msg == "err") {
		cerr << "ERROR: Unable to create account\n";
		return -3;
	}*/
	//close(sock);
	return 0;
}


int main(int argc, char **argv) {
	
	//OPTIONAL: Precheck if arguments are valid
	string client_info, client_ip, client_port, tracker_file, tracker_info, tracker_ip, tracker_port, info;
	client_info = argv[1];
	tracker_file = argv[2];
	client_ip = "";
	for(int i=0;i<client_info.length();i++) {
		if(client_info.at(i) == ' ') continue;
		if(client_info.at(i) == ':') break;
		client_ip+=client_info.at(i);
	}
	int cport, tport;
	stringstream client_str(client_info.substr(client_info.find(":") + 1)); 
	client_str >> cport;
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
	
	pthread_t runServer;
	pthread_create(&runServer, 0, client_as_server, (void *)&client_info);
	
	//Create socket for message passing
	struct sockaddr_in server_addr;
	int sock = 0, valread; 
    	struct sockaddr_in serv_addr; 

    	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  { 
		printf("\n Socket creation error \n"); 
		return -1; 
   	} 
   
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(tport);
       
    	// Convert IPv4 and IPv6 addresses from text to binary form 
    	if(inet_pton(AF_INET, tracker_ip.c_str(), &serv_addr.sin_addr)<=0)  { 
        	printf("\nInvalid address/ Address not supported \n"); 
        	return -1; 
    	} 
   
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        	printf("\nConnection Failed \n"); 
        	return -1; 
    	}
	
	//Client stays alive
	
	while(true)  {
		int rcvd_msg;
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
			string message = "create_user:"+user_id+":"+password+":"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			cout<<"Creating account..."<<endl;
			send(sock, send_buf, strlen(send_buf), 0);
			cout << "User "+user_id+" created" << endl;

			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));

		}
		
		else if(vec_command[0] == "login") {
			string user_id = vec_command[1];
			string password = vec_command[2];
			string message = "login:"+user_id+":"+password+":"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			send(sock, send_buf, strlen(send_buf), 0);
	
			cout<<"Logging in..."<<endl;

			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));

		}
		
		else if(vec_command[0] == "create_group") {
			string groupid = vec_command[1];
			string message = "create_group:"+groupid+":"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			cout<<"Creating group..."<<endl;
			send(sock, send_buf, strlen(send_buf), 0);
			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));
		}
		
		else if(vec_command[0] == "join_group") {
			string groupid = vec_command[1];
			string message = "join_group:"+groupid+":"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			send(sock, send_buf, strlen(send_buf), 0);
	
			cout<<"Joining group..."<<endl;

			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));
		}
		
		else if(vec_command[0] == "leave_group") {
			string groupid = vec_command[1];
			string message = "leave_group:"+groupid+":"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			send(sock, send_buf, strlen(send_buf), 0);
	
			cout<<"Leaving group..."<<endl;

			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));
		}
		
		if(vec_command[0] == "list_groups") {
			string message = "list_groups:"+client_ip+":"+client_port;
			char send_buf[1024]= {0};
			char rec_buf[1024] = {0};
			strcpy(send_buf,message.c_str());
			//printf("%s\n",send_buf);
			cout<<"Listing groups"<<endl;
			send(sock, send_buf, strlen(send_buf), 0);

			rcvd_msg = read(sock, rec_buf, strlen(rec_buf));
		}
		
		else if(vec_command[0] == "list_files") {
			
		}
		
		else if(vec_command[0] == "upload_file") {
			
		}
		
		else if(vec_command[0] == "download_file") {
		
		}
	
	}
	return 0;
}
