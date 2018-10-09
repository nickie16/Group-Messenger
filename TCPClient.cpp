#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/algorithm/string/predicate.hpp> 
#include "TCPClient.h"

#define PORT 8080 
#define SERVER_ADDR "192.168.56.101"
#define USERNAME "nikmand"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

	
Client::Client(string ip_addr, int netport, string name){
   ip = ip_addr;
   port = netport;
   username = name;
}

Client::~Client(){
;
}

/*
ssize_t Client::init(){
   memset(&serv_addr, '0', sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
   serv_addr.sin_port = htons(PORT);
   rgstr();
   return 0;	
}*/
	
ssize_t Client::connectToServer()
{
   struct sockaddr_in serv_addr;
   
   memset(&serv_addr, '0', sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
   serv_addr.sin_port = htons(PORT);
   
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
        cout << "Socket creation error " << endl;
        return -1;
   }

   // client's socket gets binded to port dynamically 
   if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
	cout << errno << endl;
	cout << "Connection Failed " << endl;
	return -1;
   }
   return 0;	
}

void Client::rgstr(){
	connectToServer();
	string command = "!r " + ip + ":" + std::to_string(port) + ":" + username;
	send(sock , command.c_str() , command.size() , 0 );
	valread = read( sock , buffer, 1024);
	id =  std::stoi(string(buffer, valread));
}

void Client::list_groups(){
}

void Client::list_members(string group_name){
}

void Client::join_group(string group_name){
}

void Client::quit_group(string group_name){
}

void Client::quit(){
   cout << id << endl;
}

void Client::set_group(string group_name){
	cur_group = group_name;
}

void Client::sendCommand(string input){
	connectToServer();
	send(sock ,input.c_str(), input.size() ,0);
        valread = read(sock ,buffer , 1024);
	string tmp = string(buffer, valread);
	if (boost::starts_with(input,"!lm")){
	   std::stringstream out;
	   out.str(tmp);
	   std::list<string> newlist;
     	   boost::archive::binary_iarchive ia(out);
    	   ia >> newlist;
	   for (auto v: newlist)
		cout << v << endl;
	}
	else 
	   cout << tmp << endl;
}

void Client::sendMessage(string msg){

}

string Client::getIp(){
   return ip;	
}	

string Client::getUsername(){
   return username;
}	

int Client::getPort(){
   return port;
}


