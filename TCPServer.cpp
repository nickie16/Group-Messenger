#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <iostream> 
#include <arpa/inet.h>
#include "Group.h"
#define PORT 8080 

using std::cout;
using std::endl;
using std::string;

class Server{

   private:
	int sockfd, new_socket, valread;
	char buffer[1024] = {0};
        struct sockaddr_in address;
        int addrlen = sizeof(address);	
	
   public:
	Server(){
	
	}
	
	~Server(){
	
	}

	ssize_t init(){
	    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
            {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1"); 
            address.sin_port = htons( PORT );
            // Forcefully attaching socket to the port 8080 
            if (bind(sockfd, (struct sockaddr *) &address,  sizeof(address))<0)
            {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
            if (listen(sockfd, 5) < 0)
            {
                perror("listen");
                exit(EXIT_FAILURE);
            }
	    // maximum number of clients waiting to the queue 3, any new requests gets rejected
	}

	void run(){
	    while (true){
		char *hello = "hello world";
                cout << "Waiting for an incoming connection..." << endl;
                if ((new_socket = accept(sockfd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0)
                {
                        perror("accept");
                        exit(EXIT_FAILURE);
                }
                cout << "Connection established.." << endl;
                if ((valread = read( new_socket , buffer, sizeof(buffer))) < 0) perror("read from remote peer failed");
                string input = std::string(buffer, valread);
		int pos = input.find(' ');
                string cmd = input.substr(0, pos);
                string group_name= input.substr(pos+1);
		cout << input << endl;
		if (cmd =="!lg") list_groups();
                else if (cmd == "!lm") list_members(group_name);
                else if (cmd == "!j")  join_group(group_name);
                else if (cmd == "!r")  rgstr(group_name);
                else if (cmd == "!e")  quit_group(group_name);
                send(new_socket , hello , strlen(hello) , 0 );
                cout << "message sent" << endl;
                if (close(new_socket) < 0) perror("close");
		cout << "Connection closed" << endl;
           }
	} 

	int rgstr(string aux){
		
	}

	void list_groups(){

	}

	void list_members(string group){

	}

	void join_group(string group){

	}

	void quit_group(string group){

	}

	void quit(int id){

	}
	

};

int main(int argc, char const *argv[]) 
{

	Server *server = new Server();
	Group *group = new Group("nikmand");
	server->init();
	server->run(); 
   	return 0; 
}
