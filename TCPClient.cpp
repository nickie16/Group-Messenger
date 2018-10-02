#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <iostream> 
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
	
ssize_t Client::init(){
   memset(&serv_addr, '0', sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
   serv_addr.sin_port = htons(PORT);
   //client_addr.sin_family = AF_INET;
   //client_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
   //client_addr.sin_port = htons(8081);
   rgstr();
   return 0;	
}
	
ssize_t Client::connectToServer(){
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
        cout << "\n Socket creation error \n";
        return -1;
   }
   // client's socket gets binded to port dynamically 
   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
   {
	cout << errno << endl;
	printf("\nConnection Failed \n");
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
	string command = "hello world";
	send(sock , command.c_str() , command.size() , 0 );
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
	send(sock , input.c_str() , input.size() , 0 );
        valread = read( sock , buffer, 1024);
        cout << string(buffer, valread) << endl;

}

void Client::sendMessage(string msg){

}	

string Client::getUsername(){
	return username;
}	

