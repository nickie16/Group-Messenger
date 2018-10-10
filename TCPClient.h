
#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <errno.h>

using std::string;

class Client{

   private:
	string ip;
	string username;
	string cur_group;
	int port;
	int id;
	int sock;
	int valread;
	struct sockaddr_in serv_addr;
   	char buffer[1024] = {0};

   public:	
	Client(string ip_addr, int netport, string name);

	~Client();
	
	ssize_t connectToServer();

	ssize_t init();

	void rgstr();
	
	void list_groups();

	void list_members(string group_name);

	void join_group(string group_name);

	void quit_group(string group_name);

	void quit();

	void set_group(string group_name);

	void sendCommand(string input);
	
	void sendMessage(string msg);
	
	string getIp();
   		
	string getUsername();

	int getPort();
};

#endif
