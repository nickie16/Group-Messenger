#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <iostream> 
#include <errno.h>

#define PORT 8080 
#define SERVER_ADDR "192.168.56.101"
#define USERNAME "nikmand"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

class Client{

   private:
	string ip, username, cur_group;
	int port, id, sock, valread;
	struct sockaddr_in serv_addr, client_addr;
   	char buffer[1024] = {0};

   public:	
	Client(string ip_addr, int netport, string name){
	   ip = ip_addr;
	   port = netport;
	   username = name;
	}

	~Client(){
	
	}
	
	ssize_t init(){
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
	
	ssize_t connectToServer(){
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

	void rgstr(){
		connectToServer();
		string command = "!r " + ip + ":" + std::to_string(port) + ":" + username;
		send(sock , command.c_str() , command.size() , 0 );
		valread = read( sock , buffer, 1024);
		cout << string(buffer, valread) << endl;
		cout << valread << endl;
	}
	
	void list_groups(){
		string command = "hello world";
		send(sock , command.c_str() , command.size() , 0 );
	}

	void list_members(string group_name){
	}

	void join_group(string group_name){
	}

	void quit_group(string group_name){
	}

	void quit(){
	}

	void set_group(string group_name){
		cur_group = group_name;
	}

	void sendCommand(string input){
		connectToServer();
		send(sock , input.c_str() , input.size() , 0 );
                valread = read( sock , buffer, 1024);
                cout << string(buffer, valread);

	}
	
	void sendMessage(string msg){

	}	
	
   	string getUsername(){
		return username;
	}	
};

int main(int argc, char const *argv[]) 
{ 
    string input;
    ssize_t i;
    if(argc < 3){
	cout << "Specify arguments! 1. port and 2. username, default values are going to be used" << endl;
    }		
    Client *client = new Client("192.168.1.2", argc > 1 ? std::stoi(argv[1]) : PORT, argc > 2 ? argv[2] : USERNAME);
    cout << client->getUsername() << endl;
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
