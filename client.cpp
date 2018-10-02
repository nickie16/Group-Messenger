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


int main(int argc, char const *argv[]) 
{ 
    string input;
    ssize_t i;
    if(argc < 3){;
	//cout << "Specify arguments! 1. port and 2. username, default values are going to be used" << endl;
    }		
    Client *client = new Client("192.168.1.2", argc > 1 ? std::stoi(argv[1]) : PORT, argc > 2 ? argv[2] : USERNAME);
    if ((i = client->init()) < 0) return i;
    cout << '>';
    getline(cin, input);
    while (input != "!q"){ 
    	if (input.find('!') == 0){
		int pos = input.find(' ');
		string cmd = input.substr(0, pos);
		string group_name= input.substr(pos+1);
		client->sendCommand(input);
		/*if (cmd =="!lg") client->list_groups();
		else if (cmd == "!lm") client->list_members(group_name);
		else if (cmd == "!j")  client->join_group(group_name);
		else if (cmd == "!w")  client->set_group(group_name);
		else if (cmd == "!e")  client->quit_group(group_name);*/
		cout << "Command Mode..." << endl;
	}
	else {
		client->sendMessage(input);
	}	
	cout << '>' ;
	getline(cin, input);
    }
    client->quit();
    cout << "Bye..." << endl;	
    delete client;
    return 0; 
}
