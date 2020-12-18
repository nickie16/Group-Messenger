#include <unistd.h>
#include <iostream>
#include "TCPClient.h"

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define USERNAME "nikmand"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;

//void init_logging()
//{
//    logging::core::get()->set_filter
//            (
//                    logging::trivial::severity >= logging::trivial::info
//            );
//
//    logging::add_common_attributes();
//}

void check_input(string input){
    // TODO validate input
    return ;
}

int main(int argc, char const *argv[]) {
    string input;
    ssize_t i;

    if (argc < 3) { ;
        cout << "Specify arguments! 1. port and 2. username, default values are going to be used" << endl;
    }
    // TODO infer ip address
    auto *client = new Client("192.168.1.2", argc > 1 ? std::stoi(argv[1]) : PORT, argc > 2 ? argv[2] : USERNAME);
    //if ((i = client->init()) < 0) return i;
    client->init();
    cout << '>';
    getline(cin, input);
    while (input != "!q") {
        // check_input(input);
        if (input.find('!') == 0) { //  boost::starts_with(input,'!')

            cout << "Command Mode..." << endl;
            client->sendCommand(input);
            /*if (cmd =="!lg") client->list_groups();
            else if (cmd == "!lm") client->list_members(group_name);
            else if (cmd == "!j")  client->join_group(group_name);
            else if (cmd == "!w")  client->set_group(group_name);
            else if (cmd == "!e")  client->exit_group(group_name);*/
        } else {
            client->sendMessage(input);
        }
        cout << '>';
        getline(cin, input);
    }
    client->quit();
    cout << "Bye..." << endl;
    delete client;
    return 0;
}
