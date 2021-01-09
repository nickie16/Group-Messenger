
#ifndef GROUP_MESSENGER_COMMON_H
#define GROUP_MESSENGER_COMMON_H

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>

using std::map;
using std::string;
using std::list;

enum controlMessageType { register_client, list_groups, list_members, join_group,
        exit_group, quit};

//map<string, controlMessageType> message_type_map = {
//        {"!r", register_client},
//        {"!lg", list_groups}
//};

//template <typename T>


#endif
