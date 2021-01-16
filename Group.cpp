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
	// TODO avoid pushing a client more than once
	members.push_back(t);
}

void Group::removeMemberByName(const string& member_name)
{
	auto is_ = [member_name](ClientEntry &t) { return t.getUsername() == member_name;};
	members.remove_if(is_);
}

void Group::printMembers(){
	cout << "Number of members in group: " << members.size() << endl;
	for (const auto& member : members){
	     cout << member.getUsername() << endl;
	}
}

void Group::removeMember(int user_id)
{
    auto is_ = [user_id](ClientEntry &t) { return t.getId() == user_id;};
    members.remove_if(is_);
}
