#include <unistd.h>
#include <iostream>
#include <atomic>
#include "TCPClient.h"

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;


void get_ip(char* buffer){
    const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv{};
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0)
    {
        std::cout << "Socket error" << std::endl;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0)
    {
        std::cout << "Error number: " << errno
                  << ". Error message: " << strerror(errno) << std::endl;
    }

    struct sockaddr_in name{};
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    inet_ntop(AF_INET, &name.sin_addr, buffer, 80);

    close(sock);
}

void check_input(string input){
    // TODO validate input
    return ;
}



int main(int argc, char const *argv[]) {
    char login_user[1024], ip[80];
    string input, username;
    int port;
    std::atomic<bool> should_thread_exit (false);

    if (argc < 3) { ;
        cout << "Specify arguments! 1. port and 2. username, default values are going to be used" << endl;
    }

    get_ip(ip);
    getlogin_r(login_user, sizeof(login_user));
    port = argc > 1 ? std::stoi(argv[1]) : PORT;
    username = argc > 2 ? argv[2] : login_user;

    auto *client = new Client(ip, port,  username);
//    cout << login_user << endl;
//    cout << ip << endl;
    client->init();
    client->start_udp_thread(should_thread_exit);

    cout << '[' << username << ']' << '>';
    getline(cin, input);
    while (input != "!q") {
        // check_input(input);
        if (input.find('!') == 0) { //  boost::starts_with(input,'!')

            cout << "Command Mode..." << endl;
            client->sendCommand(input);

        } else {
            client->sendMessage(input);
        }
        cout << '[' << username << ']' << '>';
        getline(cin, input);
    }
    client->quit();
    client->stop_udp_thread(should_thread_exit);

    cout << "Bye..." << endl;
    delete client;
    return 0;
}
