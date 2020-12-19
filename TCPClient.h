
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
    int port, id{};
    int sock_tcp{}, sock_udp{};
    int valread{}, valread_udp{};
    Group *currentGroup{};
    struct sockaddr_in serv_addr{}, cln_address{};
    char buffer[1024] = {0}, buffer_udp[1024] = {0};

    void unicast(Client* t);

public:
    Client(string ip_addr, int netport, string name);

    ~Client();

    string getIp();

    string getUsername();

    int getPort() const;

    ssize_t connect_to_server();

    ssize_t init();

    void register_to_server();

    void init_udp();

    void receive_udp();

    static void list_groups(const string& reply);

    static void list_members(const string& group_name);

    void join_group(const string& group_name);

    void exit_group(const string& group_name);

    void quit();

    void set_group(Group *group_name);

    void sendCommand(const string& input);

    void sendMessage(string msg);

    // send to client method? will it be useful?
};

#endif
