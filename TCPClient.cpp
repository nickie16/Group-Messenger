#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>
#include <list>
#include <boost/algorithm/string/predicate.hpp>
#include <utility>
#include <thread>
#include <csignal>
#include <atomic>
#include "TCPClient.h"
#include "ControlMessage.h"
#include "GroupMessage.h"
#include "Group.h"


#define PORT 8091
#define SERVER_ADDR "127.0.0.1"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::list;


Client::Client(string ip_addr, int netport, string name) {

    ip = std::move(ip_addr);
    port = netport;
    username = std::move(name);

    cout << "Creating client with name " << username << " in ip " << ip << ", port " << port << "." << endl;
}

Client::~Client() {
    // TODO check what is needed in destruct
    ;
}


ssize_t Client::init() {
    //initialize of client's cln_addr should happens only once
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

    // Creating socket file descriptor for receiving messages
    if ((sock_udp_recv = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    cln_addr.sin_family = AF_INET;
    cln_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // inet_addr("127.0.0.1");
    cln_addr.sin_port = htons(port);

    // Forcefully attaching receiving socket to port
    if (bind(sock_udp_recv, (struct sockaddr *) &cln_addr, sizeof(cln_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Creating socket file descriptor for sending messages
    if ((sock_udp_send = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

}

void Client::sendUdpMessage(const ClientEntry& client_entry, const string& message) {

    cln_addr.sin_family = AF_INET;
    cln_addr.sin_addr.s_addr = inet_addr(client_entry.getIp().c_str());
    cln_addr.sin_port = htons(client_entry.getPort());

    GroupMessage group_message(username, message);
    string serialized_message = serialize_object(group_message);
    sendto(sock_udp_send, serialized_message.c_str(), serialized_message.size(),MSG_CONFIRM,
           (const struct sockaddr *) &cln_addr,sizeof(cln_addr));
}

void Client::receive_udp() {

    valread_udp = read(sock_udp_recv, buffer_udp, sizeof(buffer));

    if (valread_udp >= 0) {
        string reply = string(buffer_udp, valread_udp);
        GroupMessage group_message = deserialize_object<GroupMessage>(reply);

        cout << endl << "in " << currentGroup.getName() << ' ' << group_message.getUsername() << " says: " <<
            group_message.getMessage() << endl;

        // TODO implement FIFO and Total Ordering
    } else {
        cout << "Issue" << endl;
    }
}

void Client::start_udp_thread(std::atomic<bool>& should_thread_exit) {

    auto udp_loop = [](Client* t , std::atomic<bool>* should_thread_exit){
        siginterrupt(SIGINT, true);
        std::signal(SIGINT, Client::signalHandler);

        while(!(*should_thread_exit)) {
            t->receive_udp();
        }
    };

    thread_udp = std::thread(udp_loop, this, &should_thread_exit);
}

void Client::stop_udp_thread(std::atomic<bool>& should_thread_exit){
    should_thread_exit = true;
    pthread_kill(thread_udp.native_handle(), SIGINT); // signal is send to interrupt the
    thread_udp.join();
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

    //std::unique_ptr<ControlMessage> ctrl_message( new ControlMessage(0, username, "!r", ip + ":" + std::to_string(port)));

    ControlMessage ctrl_message(id, username, "!r", ip + ":" + std::to_string(port) );
    cout << "Id: "<< ctrl_message.getId() << endl;
    string ctrl_message_serialized = serialize_object(ctrl_message);

    send(sock_tcp, ctrl_message_serialized.c_str(), ctrl_message_serialized.size(), 0);
    valread = read(sock_tcp, buffer, sizeof(buffer));
    id = std::stoi(string(buffer, valread));
    cout << "Successfully registered to Server. Acquired id_user: " << id << endl;
}

void Client::process_command(const string& input) {

    cout << "Start processing command message" << endl;

    int pos = input.find(' '); //TODO if no space what happens ?
    string cmd = input.substr(0, pos);
    string param = input.substr(pos + 1);

    if (boost::starts_with(input,"!w")) {
        set_group(param);
    } else {

        connect_to_server();
        //controlMessageType command = message_type_map[cmd];

        ControlMessage ctrl_message(id, username, cmd, param);
        string ctrl_message_serialized = serialize_object(ctrl_message);

        send(sock_tcp, ctrl_message_serialized.c_str(), ctrl_message_serialized.size(), 0);
        valread = read(sock_tcp, buffer, sizeof(buffer));
        string reply = string(buffer, valread);

        if (boost::starts_with(input, "!lg")) {
            list_groups(reply);
        } else if (boost::starts_with(input, "!lm")) {
            list_members(reply);
        } else if (boost::starts_with(input, "!j")) {
            join_group(reply);
        } else if (boost::starts_with(input, "!e")) {
            exit_group(reply);
        } else
            cout << reply << endl;
    }
}

void Client::list_groups(const string& reply) {
    cout << "List of groups received!" << endl;

    list<string> groupNames = deserialize_object<list<string> >(reply);
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
        list<string> memberNames = deserialize_object<list<string>>(reply);
        if (memberNames.empty()) {
            cout << "This group doesn't contain any member" << endl;
        } else {
            for (const auto &memberName: memberNames) {
                cout << memberName << endl;
            }
        }
    }
    catch (cereal::Exception& e){
        cout << "Group does not exists!" << endl;
    }

}

void Client::join_group(const string& group_serialized) {

    Group group = deserialize_object<Group>(group_serialized);

    std::pair<string, Group> pair(group.getName(), group);
    groups.insert(pair);
}

void Client::exit_group(const string& group_name) {
}

void Client::quit() {
    // TODO send quiting to server
    cout << "Client with id_user " << id << "is exiting." << endl;
}

void Client::set_group(string group_name) {
    currentGroup = groups.at(group_name);
}

void Client::sendMessage(string message) {

    auto current_group_client_entries = currentGroup.getMembers();
    for (auto client_entry: current_group_client_entries){
        sendUdpMessage(client_entry, message);
    }
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

void Client::signalHandler(int signum) {

    cout << "Interrupt signal (" << signum << ") received.\n";

}
