
#ifndef GROUP_H
#define GROUP_H

class Group; // forward declaration
#include <iostream>
#include <string>
#include <list>
#include <atomic>
#include "TCPClient.h"

using std::list;
using std::string;

class Group {

private:
    string name;
    list<Client> members;
public:
    explicit Group(string group);

    ~Group();

    string getName();

    list<Client> getMembers();

    void addMember(const Client& t);

    void printMembers();

    void removeMember(const string& member_name);
};

#endif
