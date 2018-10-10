
#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <string>
#include <list>
#include "TCPClient.h"

using std::list;
using std::string;

class Group{

	private:
		string name;
		list<Client> members;
	public:
		Group(string group);
		~Group();
		string getName();
		list<Client> getMembers();
		void addMember(Client t);
		void printMembers();
		void removeMember(string name);
};

#endif
