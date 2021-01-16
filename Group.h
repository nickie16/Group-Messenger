
#ifndef GROUP_H
#define GROUP_H

//class Group; // forward declaration
#include <iostream>
#include <string>
#include <list>
#include <atomic>
#include "common.h"
#include "ClientEntry.h"

using std::list;
using std::string;
using std::cout;

class Group {

private:
    string name;
    list<ClientEntry> members; // TODO maybe a hashmap instead of a list? indeed it will be useful

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive( name, members);
    }


public:
    Group() = default;

    explicit Group(string group);

    ~Group();

    string getName();

    list<ClientEntry> getMembers();

    void addMember(const ClientEntry& t);

    void printMembers();

    void removeMember(int user_id);

    void removeMemberByName(const string& member_name);
};

#endif
