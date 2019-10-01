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
#include "TCPClient.h"


#define PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define USERNAME "nikmand"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::list;

list<string> deserialize_list(string reply){
    std::stringstream out;
    out.str(reply);
    list<string> newlist;
    boost::archive::binary_iarchive ia(out);
    ia >> newlist;
    return newlist;
}

Client::Client(string ip_addr, int netport, string name) {
    ip = ip_addr;
    port = netport;
    username = name;
}

Client::~Client() {
    ;
}


ssize_t Client::init() {
    //initialize of client's address should happens only once
    cout << "Initializing Client" << endl;

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serv_addr.sin_port = htons(PORT);
    registerToServer();
    return 0;
}

ssize_t Client::connectToServer() {
    cout << "Starting connect" << endl;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error " << endl;
        return -1;
    }

    // client's socket gets bound to port dynamically
    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << errno << endl;
        cout << "Connection Failed " << endl;
        return -1;
    }

    cout << "Connection Achieved" << endl;
    return 0;
}

void Client::registerToServer() {
    cout << "Attempt to register on Server" << endl;
    connectToServer();
    string command = "!r " + ip + ":" + std::to_string(port) + ":" + username;
    send(sock, command.c_str(), command.size(), 0);
    valread = read(sock, buffer, 1024);
    id = std::stoi(string(buffer, valread));
    cout << "Succesfully registed to Server. Acquired id: " << id << endl;
}

void Client::list_groups(string reply) {
    std::list<string> groupNames = deserialize_list(std::move(reply));
    if (groupNames.empty()) {
        cout << "No groups have been created so far" << endl;
    } else {
        for (const auto& groupName: groupNames) {
            cout << groupName << endl;
        }
    }
}

void Client::list_members(string reply) {
    std::list<string> memberNames = deserialize_list(reply);
    if (memberNames.empty()) {
        cout << "This group doesn't contain any member" << endl;
    } else {
        for (const auto& memberName: memberNames) {
            cout << memberName << endl;
        }
    }
}

void Client::join_group(string groupObject) { // TODO the whole group object is returned
}

void Client::quit_group(string group_name) {
}

void Client::quit() {
    cout << id << endl;
}

void Client::set_group(Group *group_name) {
    currentGroup = group_name;
}

void Client::sendCommand(string input) {

    cout << "Start sending command" << endl;
    connectToServer();
    send(sock, input.c_str(), input.size(), 0);
    valread = read(sock, buffer, 1024);
    string tmp = string(buffer, valread);
    if (boost::starts_with(input, "!lg")) {
        list_groups(tmp);
    } else
        cout << tmp << endl;
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

int Client::getPort() {
    return port;
}


