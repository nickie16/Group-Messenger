
#ifndef CLIENT_H
#define CLIENT_H

#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <cerrno>
//#include "Group.h"
class Group;

using std::string;

class Client {

private:
    string ip;
    string username;
    int port;
    int id;
    int sock;
    int valread;
    Group *currentGroup;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

public:
    Client(string ip_addr, int netport, string name);

    ~Client();

    string getIp();

    string getUsername();

    int getPort();

    ssize_t connectToServer();

    ssize_t init();

    void registerToServer();

    static void list_groups(string reply);

    void list_members(string group_name);

    void join_group(const string& group_name);

    void quit_group(const string& group_name);

    void quit();

    void set_group(Group *group_name);

    void sendCommand(const string& input);

    void sendMessage(string msg);

    // send to client method? will it be useful?
};

#endif
