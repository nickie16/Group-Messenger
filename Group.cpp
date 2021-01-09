#include "Group.h"

//#include <utility>

using std::endl;

Group::Group(string group){
	name = std::move(group);
}

Group::~Group(){
;
}

string Group::getName() {
	return name;
}

list<ClientEntry> Group::getMembers(){

	return members;
}

void Group::addMember(const ClientEntry& t){
	members.push_back(t);
}

void Group::removeMember(const string& member_name)
{
	auto is_ = [member_name](ClientEntry &t) { return t.getUsername() == member_name;};
	members.remove_if(is_);
}

void Group::printMembers(){
	std::cout << "Number of members in group: " << members.size() << endl;
	for (const auto& member : members){
	     std::cout << member.getUsername() << endl;
	     std::cout << "inside printMembers" << endl;
	}
} 
