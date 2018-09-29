#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <iostream> 
#include <arpa/inet.h>
#define PORT 8080 

using std::cout;
using std::endl;

int main(int argc, char const *argv[]) 
{

	int sockfd, new_socket, valread;
	struct sockaddr_in address; 
    	int addrlen = sizeof(address); 
    	char buffer[1024] = {0};
	char *hello = "Hello from server"; 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        	perror("socket failed"); 
        	exit(EXIT_FAILURE); 
    	}
	address.sin_family = AF_INET; 
        address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
        address.sin_port = htons( PORT );
    	// Forcefully attaching socket to the port 8080 
    	if (bind(sockfd, (struct sockaddr *) &address,  sizeof(address))<0) 
    	{ 
        	perror("bind failed"); 
        	exit(EXIT_FAILURE); 
    	}
    	if (listen(sockfd, 3) < 0) 
    	{ 
        	perror("listen"); 
        	exit(EXIT_FAILURE); 
    	}  
	// maximum number of clients waiting to the queue 3, any new requests gets rejected
    	if ((new_socket = accept(sockfd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) 
    	{ 
        	perror("accept"); 
        	exit(EXIT_FAILURE); 
    	} 
    	valread = read( new_socket , buffer, 1024); 
    	cout << buffer << endl; 
    	send(new_socket , hello , strlen(hello) , 0 ); 
    	cout << "Hello message sent" << endl; 
   	return 0; 
}
