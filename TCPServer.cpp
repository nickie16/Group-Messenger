#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <unordered_map>
#include <list>
#include <sstream>
#include <atomic>
#include "Group.h"
#include "common.h"
#include "ControlMessage.h"

#define PORT 8091

using std::list;
using std::cout;
using std::endl;
using std::string;


class Server {

private:
    int sockfd{}, client_socket{}, valread{};
    char buffer[1024] = {0};
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    list<Group*> chatRooms;
    std::unordered_map<int, ClientEntry> client_enties;
    int num_of_clients = 0;

    Server() = default;

public:
    static Server &getInstance() {
        static Server instance; // Guaranteed to be destroyed. Instantiated on first use.
        return instance;
    }

    Server(Server const &) = delete;

    void operator=(Server const &) = delete; // we forbid the use of this function as Server is singleton

    // TODO delete objects

    void init() {

        cout << "Initializing our Server" << endl;

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to port
        if (bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(sockfd, 5) < 0) { // maximum number of clients waiting to the queue 5, any new requests gets rejected
            perror("listen failed");
            exit(EXIT_FAILURE);
        }

        cout << "Initialization finished" << endl;
    }

    void run() {
        while (true) {
            cout << "Waiting for an incoming connection..." << endl;

            if ((client_socket = accept(sockfd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
                perror("accept failed");
                exit(EXIT_FAILURE);
            }
            cout << "Connection established.." << endl;
            cout << "Connected to: " << inet_ntoa(address.sin_addr) << endl;
            cout << "Connection port: " << address.sin_port << endl;

            // because communication is synchronous we respond right away to the correct client using client_socket
            if ((valread = read(client_socket, buffer, sizeof(buffer))) < 0) {
                perror("read from remote peer failed");
            }

            string input = std::string(buffer, valread);

            ControlMessage ctrl_message = deserialize_object<ControlMessage>(input);

            // TODO if command is not register check if user is registered
            int user_id = ctrl_message.getId();
            string username = ctrl_message.getUsername();
            string cmd = ctrl_message.getMessageType();
            string param = ctrl_message.getParams();
            //cout << "Command received was: "  << input << endl;
            cout << cmd << endl;
            cout << param << endl;
            // TODO command codes should be moved as constants in another file
            if (cmd == "!lg")
                list_groups();
            else if (cmd == "!lm")
                list_members(param);
            else if (cmd == "!j")
                join_group(param, user_id);
            else if (cmd == "!r")
                registerClient(param, username);
            else if (cmd == "!e")
                exit_group(param, user_id);
            else if (cmd == "!q")
                quit_messenger(user_id);
            else
                cout << "Unsupported Command received" << endl; // TODO implement reply unsupported command
           // cout << "Reply sent" << endl;

            // closing the tcp connection as it is expensive to maintain
            if (close(client_socket) < 0) {
                perror("close");
            }
            cout << "Connection closed" << endl;
        }
    }

    void registerClient(const string& client_info, string username) {
        cout << "Registering new Client" << endl;

        int id = ++num_of_clients;
        int pos = client_info.find(':');
        string ip = client_info.substr(0, pos);
        int port = stoi(client_info.substr(pos + 1));

        ClientEntry client_entry = ClientEntry(id, username, ip, port);

        std::pair<int, ClientEntry> pair(id, client_entry);
        client_enties.insert(pair);
        string ids = std::to_string(id);
        send(client_socket, ids.c_str(), ids.size(), 0);
    }

    void list_groups() {
        cout << "Fetching groups" << endl;

        list<string> nameList;
        for (auto & chatRoom : chatRooms) {
            nameList.push_back(chatRoom->getName());
        }
        string nameListSerialized = serialize_object(nameList);
        send(client_socket, nameListSerialized.c_str(), nameListSerialized.size(), 0);
    }

    void list_members(const string& groupName) {
        Group *group;

        group = find_group(groupName);
        if (group){
            list<string> nameList;
            for (const auto& v : group->getMembers()) {
                nameList.push_back(v.getUsername());
            }
            cout << "Number of members on the requested group: " << nameList.size() << endl;
            string nameListSerialized = serialize_object(nameList);
            send(client_socket, nameListSerialized.c_str(), nameListSerialized.size(), 0);
        }
        // NOTE client handles the case when group is not present
    }

    void join_group(const string& groupName, int user_id) {
        Group *group;

        group = find_group(groupName);
        if (!group){
            group = new Group(groupName);
            chatRooms.push_back(group);
        }

        ClientEntry t = client_enties.at(user_id);
        group->addMember(t);

        string group_serialized = serialize_object(*group);
        send(client_socket, group_serialized.c_str(), group_serialized.size(), 0);
    }

    void exit_group(const string& groupName, int user_id) {
        Group *group;

        group = find_group(groupName);
        if (group){
            group->removeMember(user_id);
        }
    }

    Group* find_group(const string& groupName){
        Group *group = nullptr;
        cout << "Searching if requested group exists" << endl;

        for (auto & chatRoom : chatRooms) {
            if (chatRoom->getName() == groupName) {
                cout << "Requested group found" << endl;

                group = chatRoom;
                return group;
            }
        }

        cout << "There isn't any group named: " << groupName << endl;
        return group;
    }

    void quit_groups(int user_id) {
        for(auto group : chatRooms){
            group->removeMember(user_id);
        }
    }

    void quit_messenger(int user_id) {
        quit_groups(user_id);
        client_enties.erase (user_id);
        // TODO update the group list of other clients, is it possible?
    }
};

int main(int argc, char const *argv[]) {

    Server &server = Server::getInstance();
    server.init();
    server.run();  // TODO check how we can properly shutdown the server
    cout << "Are we going to ever arrive here?" << endl;
    return 0;
}
