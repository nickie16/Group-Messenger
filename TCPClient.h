
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
#include <thread>
#include <cerrno>
#include "common.h"
#include "ClientEntry.h"
#include "Group.h"

//class Group;

using std::string;

class Client {

private:
    string ip;
    string username;
    int port, id{};
    int sock_tcp{}, sock_udp_recv{}, sock_udp_send{};
    int valread{}, valread_udp{};
    std::unordered_map<string, Group> groups;
    Group currentGroup{};
    struct sockaddr_in serv_addr{}, cln_addr{};
    char buffer[1024] = {0}, buffer_udp[1024] = {0};
    std::thread thread_udp;
    static void signalHandler(int signum);
    void sendUdpMessage(const ClientEntry& client_entry, const string& message);

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

    void set_group(string group_name);

    void process_command(const string& input);

    void sendMessage(string message);

    // send to client method? will it be useful?
};

#endif
