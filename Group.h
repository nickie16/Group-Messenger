
#ifndef GROUP_H
#define GROUP_H

//class Group; // forward declaration
#include <iostream>
#include <string>
#include <list>
#include <atomic>
#include "ClientEntry.h"

using std::list;
using std::string;

class Group {

private:
    string name;
    list<ClientEntry> members; // TODO maybe a hashset instead of a list?
public:
    explicit Group(string group);

    ~Group();

    string getName();

    list<ClientEntry> getMembers();

    void addMember(const ClientEntry& t);

    void printMembers();

    void removeMember(const string& member_name);
};

#endif
