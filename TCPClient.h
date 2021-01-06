
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
#include<thread>
#include <cerrno>

class Group;

using std::string;

class Client {

private:
    string ip;
    string username;
    int port, id{};
    int sock_tcp{}, sock_udp_recv{}, sock_udp_send{};
    int valread{}, valread_udp{};
    Group *currentGroup{};
    struct sockaddr_in serv_addr{}, cln_addr{};
    char buffer[1024] = {0}, buffer_udp[1024] = {0};
    void unicast(Client* t);
    std::thread thread_udp;
    static void signalHandler(int signum);
    void sendUdpMessage(const string& message);

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

    void start_udp_thread(std::atomic<bool>& should_thread_exit);

    void stop_udp_thread(std::atomic<bool>& should_thread_exit);

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
