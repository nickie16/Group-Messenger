#include "Group.h"
#include "TCPClient.h"

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

void Group::removeMember(string name)
{
	auto is_ = [name](Client &t) { return t.getUsername() == name;};
	members.remove_if(is_);
}

void Group::printMembers(){
	std::cout << members.size() << std::endl;
	for (auto v : members){
	     std::cout << v.getUsername() << "\n";
	     std::cout << "inside printMembers" << "\n";	
	}
} 
