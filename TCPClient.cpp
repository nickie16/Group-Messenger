#include <stdio.h> 
#include <unistd.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <arpa/inet.h>
#include <iostream> 

#define PORT 8080 
#define SERVER_ADDR "127.0.0.1"

using std::cout;
using std::cin;
using std::endl;
using std::string;

class Client{

   private:
	string ip, username, cur_group;
	int port, id, sock, valread;
	struct sockaddr_in serv_addr;
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
	   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
           {
                cout << "\n Socket creation error \n";
                return -1;
           }
           memset(&serv_addr, '0', sizeof(serv_addr));

           serv_addr.sin_family = AF_INET;
           serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
           serv_addr.sin_port = htons(PORT);
  	   return 0;	
	}
	
	ssize_t connectToServer(){
	   if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	   {
        	printf("\nConnection Failed \n");
        	return -1;
    	   }
	   return 0;	
	}

	void rgstr(){
		connectToServer();
		string command = ip + ":" + std::to_string(port) + ":" + username;
		send(sock , command.c_str() , command.size() , 0 );
		valread = read( sock , buffer, 1024);
	}
	
	void list_groups(){
		connectToServer();
	}

	void list_members(string group_name){
		connectToServer();
	}

	void join_group(string group_name){
		connectToServer();
	}

	void quit_group(string group_name){
		connectToServer();
	}

	void quit(){
		connectToServer();
	}

	void set_group(string group_name){
		cur_group = group_name;
	}
	
	void sendMessage(){

	}	
	
   	string getUsername(){
		return username;
	}	
};

int main(int argc, char const *argv[]) 
{ 
    string input;
    const char *aux;	
    ssize_t i;		
    Client *client = new Client("192.168.1.2", std::stoi(argv[1]), argv[2]);
    cout << client->getUsername() << endl;
    if ((i = client->init()) < 0) return i;
    cin >> input;
    aux = input.c_str(); 
    if (aux[0]=='!')
	cout << "Command Mode..." << endl;	
    cout << input << endl;
    delete client;
    return 0; 
}
