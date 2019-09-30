#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unordered_map>
#include "Group.h"
#include <list>
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
// Provide an implementation of serialize for std::list
#include <boost/serialization/list.hpp>

#define PORT 8080

using std::list;
using std::cout;
using std::endl;
using std::string;

class Server {

private:
    int sockfd, new_socket, valread;
    char buffer[1024] = {0};
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    list<Group> chatRooms;
    list<Group>::iterator it;
    std::unordered_map<int, string> unmap;
    int cnt = 0;
    Client *t;
    Group *g;

    Server() {};

public:
    static Server &getInstance() {
        static Server instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    Server(Server const &) = delete;

    void operator=(Server const &) = delete; // we forbid the use of this function as Server is singleton

    ssize_t init() {
        cout << "Initializing Server" << endl;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
        address.sin_port = htons(PORT);
        // Forcefully attaching socket to the port 8080
        if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(sockfd, 5) < 0) { // maximum number of clients waiting to the queue 5, any new requests gets rejected
            perror("listen");
            exit(EXIT_FAILURE);
        }
    }

    void run() {
        while (true) {
            char *hello = "hello world";
            cout << "Waiting for an incoming connection..." << endl;
            if ((new_socket = accept(sockfd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            cout << "Connection established.." << endl;
            cout << "Connected to: " << inet_ntoa(address.sin_addr) << endl;
            cout << "Connection port" << address.sin_port << endl;
            if ((valread = read(new_socket, buffer, sizeof(buffer))) < 0) {
                perror("read from remote peer failed");
            }
            string input = std::string(buffer, valread);
            int pos = input.find(' ');
            string cmd = input.substr(0, pos);
            string group_name = input.substr(pos + 1);
            cout << "Command received was: "  << input << endl;
            // TODO command codes should be moved as constants in another file
            if (cmd == "!lg")
                list_groups();
            else if (cmd == "!lm")
                list_members(group_name);
            else if (cmd == "!j")
                join_group(group_name);
            else if (cmd == "!r")
                registerClient(group_name);
            else if (cmd == "!e")
                quit_group(group_name);
            //send(new_socket , hello , strlen(hello) , 0 );
            cout << "Reply sent" << endl;
            if (close(new_socket) < 0) // closing the tcp connection
                perror("close");
            cout << "Connection closed" << endl;
        }
    }

    void registerClient(string aux) { // TODO rename aux
        cout << "Registering new Client" << endl;
        int id = ++cnt;
        std::pair<int, string> pair(id, aux);
        unmap.insert(pair);
        string ids = std::to_string(id);
        send(new_socket, ids.c_str(), ids.size(), 0);
    }

    void list_groups() { // TODO we should send a list of strings with the group names on it
        //string aux = "groups: ";
        list<string> nameList;
        std::stringstream nameListStream;
        for (it = chatRooms.begin(); it != chatRooms.end(); it++) {
            nameList.push_back(it->getName());
            //aux += "[" + it->getName() + "] ";
        }
        boost::archive::binary_oarchive oa(nameListStream);
        oa << nameList;
        string nameListSerialized = nameListStream.str();
        send(new_socket, nameListSerialized.c_str(), nameListSerialized.size(), 0);
    }

    void list_members(string groupName) {
        Group *group;
        cout << "we are here" << endl;
        for (it = chatRooms.begin(); it != chatRooms.end(); it++) { //TODO check why auto instead of iterator
            if (it->getName() == groupName) {
                group = &*it;
                cout << "we have a match" << endl;
                //g->printMembers();
                list<string> nameList;
                std::stringstream nameListStream;
                for (auto v : group->getMembers()) {
                    nameList.push_back(v.getUsername());
                }
                cout << "Number of members on the requested group: " << nameList.size() << endl;
                boost::archive::binary_oarchive oa(nameListStream);
                oa << nameList;
                string nameListSerialized = nameListStream.str();
                send(new_socket, nameListSerialized.c_str(), nameListSerialized.size(), 0);
                return;
            }
        }
        cout << "There isn't any group named: " << groupName << endl;
    }

    void join_group(string groupName) {
        Group *group = new Group(groupName);
        for (it = chatRooms.begin(); it != chatRooms.end(); it++) {
            if (it->getName() == groupName) {
                delete group;
                group = &*it;
                break;
            }
        }
        //TODO add user to groupName's member list
        t = new Client("127.0.0.1", 4340, "nikmand");
        group->addMember(*t);
        group->printMembers();
        if (it == chatRooms.end()) {
            chatRooms.push_back(*group);
            //perna opws einai to group ekeinh th stigmh, to add meta den kratietai, epeidh pairnei reference
        }
    }

    void quit_group(string groupName) {
        //TODO delete user from group members
        Group *group;
        for (it = chatRooms.begin(); it != chatRooms.end(); it++) {
            if (it->getName() == groupName) {
                group = &*it;
                group->removeMember("nikmand");
                return;
            }
        }
        cout << "There isn't a group named: " << groupName << endl;
    }

    void quit(int id) {
        //TODO delete id from unmap
    }
};

int main(int argc, char const *argv[]) {

    Server &server = Server::getInstance();
    //Group *group = new Group("nikmand"); // is that needed ?
    server.init();
    server.run();  // TODO check if it should run as a new thread and how can properly shutdown the server
    cout << "Are we going to ever arrive here?" << endl;
    return 0;
}
