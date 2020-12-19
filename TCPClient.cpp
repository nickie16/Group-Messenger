#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <sstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <utility>
#include "TCPClient.h"


#define PORT 8091
#define SERVER_ADDR "127.0.0.1"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::list;

list<string> deserialize_list(const string& reply){
    std::stringstream out;
    out.str(reply);
    list<string> newlist;
    boost::archive::binary_iarchive ia(out);
    ia >> newlist;
    return newlist;
}

Client::Client(string ip_addr, int netport, string name) {
    ip = std::move(ip_addr);
    port = netport;
    username = std::move(name);
}

Client::~Client() {
    // TODO check what is needed in destruct
    ;
}


ssize_t Client::init() {
    //initialize of client's cln_address should happens only once
    cout << "Initializing Client" << endl;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_port = htons(PORT);

    register_to_server();

    init_udp();

    return 0;
}

void Client::init_udp() {

    // Creating socket file descriptor
    if ( (sock_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    cln_address.sin_family = AF_INET;
    cln_address.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");
    cln_address.sin_port = htons(port);

    // Forcefully attaching socket to port
    if (bind(sock_udp, (struct sockaddr *) &cln_address, sizeof(cln_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Client::receive_udp() {

    valread_udp = read(sock_udp, buffer_udp, 1024);
    string reply = string(buffer, valread);

    // TODO figure out source of the message
    // TODO display message to screen
}

ssize_t Client::connect_to_server() {
    cout << "Starting connect" << endl;

    if ((sock_tcp = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error " << endl;
        return -1;
    }

    // client's socket gets bound to port dynamically
    if (connect(sock_tcp, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << errno << endl;
        cout << "Connection Failed " << endl;
        return -1;
    }

    cout << "Connection Achieved" << endl;
    return 0;
}

void Client::register_to_server() {
    cout << "Attempt to register on Server" << endl;
    connect_to_server();
    string command = "!r " + ip + ":" + std::to_string(port) + ":" + username;
    send(sock_tcp, command.c_str(), command.size(), 0);
    valread = read(sock_tcp, buffer, 1024);
    id = std::stoi(string(buffer, valread));
    cout << "Successfully registered to Server. Acquired id: " << id << endl;
}

void Client::sendCommand(const string& input) {

    cout << "Start sending command" << endl;
    connect_to_server();
    // TODO send also ID
    send(sock_tcp, input.c_str(), input.size(), 0);
    valread = read(sock_tcp, buffer, 1024);
    string reply = string(buffer, valread);

    // TODO implement for all possible commands
    if (boost::starts_with(input, "!lg")) {
        list_groups(reply);
    }
    else if (boost::starts_with(input, "!lm")) {
        list_members(reply);
    }
    else if (boost::starts_with(input, "!j")) {
        join_group(reply);
    }
    else if (boost::starts_with(input, "!e")) {
        exit_group(reply);
    }
    else
        cout << reply << endl;
}

void Client::list_groups(const string& reply) {
    std::list<string> groupNames = deserialize_list(reply);
    if (groupNames.empty()) {
        cout << "No groups have been created so far" << endl;
    } else {
        for (const auto& groupName: groupNames) {
            cout << groupName << endl;
        }
    }
}

void Client::list_members(const string& reply) {
    try {
        std::list<string> memberNames = deserialize_list(reply);
        if (memberNames.empty()) {
            cout << "This group doesn't contain any member" << endl;
        } else {
            for (const auto &memberName: memberNames) {
                cout << memberName << endl;
            }
        }
    }
    catch (boost::archive::archive_exception& e){
        cout << "Group does not exists!" << endl;
    }

}

void Client::join_group(const string& groupObject) { // TODO the whole group object is returned
}

void Client::exit_group(const string& group_name) {
}

void Client::quit() {
    // TODO implement
    cout << id << endl;
}

void Client::set_group(Group *group_name) {
    currentGroup = group_name;
}

void Client::sendMessage(string msg) {
    // TODO broadcast to all members of current group
    cout << "Start sending message" << endl;
}


string Client::getIp() {
    return ip;
}

string Client::getUsername() {
    return username;
}

int Client::getPort() const {
    return port;
}

void Client::unicast(Client* t) {

}


