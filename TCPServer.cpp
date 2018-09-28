#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <iostream> 
#define PORT 8080 

using std::cout;
using std::endl;

int main(int argc, char const *argv[]) 
{

	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        	perror("socket failed"); 
        	exit(EXIT_FAILURE); 
    	}
	cout << "Working!" << endl; 
}
