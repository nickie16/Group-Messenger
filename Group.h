
#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <string>
#include <list>

using std::list;
using std::string;

class Group{

	private:
		string name;
		list<string> members;
	public:
		Group(string group);
		~Group();
};

#endif
