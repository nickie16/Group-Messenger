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
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
// Provide an implementation of serialize for std::list
#include <boost/serialization/list.hpp>

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
	Client *t;
	Group *g;

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
	    // maximum number of clients waiting to the queue 5, any new requests gets rejected
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
		//cout << "Connected to: " << inet_ntoa(address.sin_addr) << endl;
		//cout << "Connection port" << address.sin_port << endl;
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
            Group *g;
	    cout << "we are here" << endl;
            for(it = chatRooms.begin(); it != chatRooms.end(); it++)
            {
                if (it->getName() == group){
                        g = &*it;
			cout << "we have a match" << endl;
			//g->printMembers();
			list<string> nameList;
		        std::stringstream touta;
        		for (auto v : g->getMembers()){
             			nameList.push_back(v.getUsername());
        		}
			cout << "List's size " << nameList.size() << endl; 
	       		boost::archive::binary_oarchive oa(touta);
			oa << nameList;
			string tmp = touta.str(); 
                        send(new_socket, tmp.c_str(), tmp.size(), 0);
                        return;
                }
            }
            cout << "There isn't any group named: " << group << endl;
	}

	void join_group(string group){
	    Group *g = new Group(group);
	    for(it = chatRooms.begin(); it != chatRooms.end(); it++)
	    {
		if (it->getName() == group){ 
			delete g;
			g = &*it;
			break;
		}
	    }
	    //TODO add user to group's member list
	    t = new Client("127.0.0.1", 4340, "nikmand");	
	    g->addMember(*t);
	    g->printMembers();
            if (it == chatRooms.end()){
                   chatRooms.push_back(*g); //perna opws einai to group ekeinh th stigmh, to add meta den kratietai, epeidh pairnei reference 
            }
	}

	void quit_group(string group){
	    //TODO delete user from group members
	    Group *g;	
	    for(it = chatRooms.begin(); it != chatRooms.end(); it++)
            {
                if (it->getName() == group){
                        g = &*it;
			g->removeMember("nikmand");
                        return;
                }
            }
	    cout << "There isn't a group named: " << group << endl;
	
	}

	void quit(int id){
	    //TODO delete id from unmap	
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
