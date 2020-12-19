#include<bits/stdc++.h>
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define TRUE 1
#define FALSE 0
#define SIZE 1024

using namespace std;

void write_file(int sockfd) {
        int n;
        FILE * fp;
        char * filename = "recv.txt";
        char buffer[SIZE];

        fp = fopen(filename, "w");
        while (1) {
                n = recv(sockfd, buffer, SIZE, 0);
                if (n <= 0) {
                        break;
                        return;
                }
                fprintf(fp, "%s", buffer);
                bzero(buffer, SIZE);
        }
        return;
}

int main() {
        char * ip = "127.0.0.1";
        int port = 8080;
        int e;
        int addrlen, new_socket, client_socket[30],
                max_clients = 30, activity, i, valread, sd;
        fd_set readfds;
        int max_sd, new_sock;
        struct sockaddr_in address;

        char * message = "ECHO Daemon v1.0 \r\n";
        int sockfd;

        struct sockaddr_in server_addr, new_addr;
        socklen_t addr_size;
        char buffer[SIZE];

        for (i = 0; i < max_clients; i++) {
                client_socket[i] = 0;
        }

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
                perror("[-]Error in socket");
                exit(1);
        }
        printf("[+]Server socket created successfully.\n");

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = port;
        server_addr.sin_addr.s_addr = inet_addr(ip);

        e = bind(sockfd, (struct sockaddr * ) & server_addr, sizeof(server_addr));
        if (e < 0) {
                perror("[-]Error in bind");
                exit(1);
        }
        printf("[+]Binding successfull.\n");

        if (listen(sockfd, 10) == 0) {
                printf("[+]Listening....\n");
        } else {
                perror("[-]Error in listening");
                exit(1);
        }

        addr_size = sizeof(new_addr);
        cout << "Waiting for clients..." << endl;
        while (true) {
                //clear the socket set  
                FD_ZERO( & readfds);

                //add master socket to set  
                FD_SET(sockfd, & readfds);
                max_sd = sockfd;

                //add child sockets to set  
                for (i = 0; i < max_clients; i++) {
                        //socket descriptor  
                        sd = client_socket[i];

                        //if valid socket descriptor then add to read list  
                        if (sd > 0)
                                FD_SET(sd, & readfds);

                        //highest file descriptor number, need it for the select function  
                        if (sd > max_sd)
                                max_sd = sd;
                }

                //wait for an activity on one of the sockets , timeout is NULL ,  
                //so wait indefinitely  
                activity = select(max_sd + 1, & readfds, NULL, NULL, NULL);

                if ((activity < 0) && (errno != EINTR)) {
                        printf("select error");
                }

                //If something happened on the master socket ,  
                //then its an incoming connection  
                if (FD_ISSET(sockfd, & readfds)) {
                        if ((new_socket = accept(sockfd,
                                        (struct sockaddr * ) & address, (socklen_t * ) & addrlen)) < 0) {
                                perror("accept");
                                exit(EXIT_FAILURE);
                        }

                        //inform user of socket number - used in send and receive commands  
                        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                        //send new connection greeting message  
                        if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
                                perror("send");
                        }

                        puts("Welcome message sent successfully");

                        //add new socket to array of sockets  
                        for (i = 0; i < max_clients; i++) {
                                //if position is empty  
                                if (client_socket[i] == 0) {
                                        client_socket[i] = new_socket;
                                        printf("Adding to list of sockets as %d\n", i);

                                        break;
                                }
                        }
                }

                //else its some IO operation on some other socket 
                for (i = 0; i < max_clients; i++) {
                        sd = client_socket[i];

                        if (FD_ISSET(sd, & readfds)) {
                                //Check if it was for closing , and also read the  
                                //incoming message  
                                if ((valread = read(sd, buffer, 1024)) == 0) {
                                        //Somebody disconnected , get his details and print  
                                        getpeername(sd, (struct sockaddr * ) & address, \
                                                (socklen_t * ) & addrlen);
                                        printf("Host disconnected , ip %s , port %d \n",
                                                inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                                        //Close the socket and mark as 0 in list for reuse  
                                        close(sd);
                                        client_socket[i] = 0;
                                }

                                //Echo back the message that came in  
                                else {
                                        //set the string terminating NULL byte on the end  
                                        //of the data read  
                                        buffer[valread] = '\0';
                                        send(sd, buffer, strlen(buffer), 0);
                                        printf("[+]Connected to Server.\n");
					  addr_size = sizeof(new_addr);
                                        new_sock = accept(sd, (struct sockaddr * ) & new_addr, & addr_size);
                                        write_file(new_sock);
                                }

                        }
                }

                printf("[+]Data written in the file successfully.\n");

        }
        return 0;
}
