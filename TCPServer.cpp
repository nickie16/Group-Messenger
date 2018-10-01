#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <iostream> 
#include <arpa/inet.h>
#include <unordered_map>
#include "Group.h"
#include <list>

#define PORT 8080

using std::list;
using std::cout;
using std::endl;
using std::string;

class Server{

   private:
	int sockfd, new_socket, valread;
	char buffer[1024] = {0};
        struct sockaddr_in address;
        int addrlen = sizeof(address);	
	list<Group> chatRooms;
	list<Group>::iterator it;
	std::unordered_map<int, string> unmap;
	int cnt = 0;

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
		cout << "Connected to: " << inet_ntoa(address.sin_addr) << endl;
		cout << "Connection port" << address.sin_port << endl;
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
                //send(new_socket , hello , strlen(hello) , 0 );
                cout << "message sent" << endl;
                if (close(new_socket) < 0) perror("close");
		cout << "Connection closed" << endl;
           }
	} 

	void rgstr(string aux){
		cout << "We are on register..."  << endl;
		int id = ++cnt;
		std::pair<int,string> zeug (id, aux);
		unmap.insert(zeug);
		string ids = std::to_string(id);
		send(new_socket, ids.c_str(), ids.size(), 0);	
	}

	void list_groups(){
	    string aux = "groups: ";
	    for(it = chatRooms.begin(); it != chatRooms.end(); it++) aux += "[" + it->getName() + "] ";
	    send(new_socket, aux.c_str(), aux.size(), 0);
	}

	void list_members(string group){

	}

	void join_group(string group){
	    Group *g;
	    for(it = chatRooms.begin(); it != chatRooms.end(); it++)
	    {
		if (it->getName() == group){ 
			g = &*it;
			break;
		}
	    }
	    if (it == chatRooms.end()){	
		   g = new Group(group);
		   chatRooms.push_back(*g);
	    }
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
