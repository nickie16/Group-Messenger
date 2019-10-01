#include "Group.h"
//#include "TCPClient.h"

using std::endl;

Group::Group(string group){
	name = group;
}

Group::~Group(){
;
}

string Group::getName() {
	return name;
}

list<Client> Group::getMembers(){
	return members;
}

void Group::addMember(Client t){
	members.push_back(t);
}

void Group::removeMember(string name) // TODO check const reference
{
	auto is_ = [name](Client &t) { return t.getUsername() == name;};
	members.remove_if(is_);
}

void Group::printMembers(){
	std::cout << "Number of members in group: " << members.size() << endl;
	for (auto member : members){
	     std::cout << member.getUsername() << endl;
	     std::cout << "inside printMembers" << endl;
	}
} 
