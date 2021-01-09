
#include <string>
#include <map>

using std::map;
using std::string;

enum controlMessageType { register_client, list_groups, list_members, join_group,
        exit_group, quit};

map<string, controlMessageType> message_type_map = {
        {"!r", register_client},
        {"!lg", list_groups}
};
