
#ifndef GROUP_MESSENGER_COMMON_H
#define GROUP_MESSENGER_COMMON_H

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <memory>
#include <cereal/archives/binary.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

using std::map;
using std::string;
using std::list;

enum controlMessageType { register_client, list_groups, list_members, join_group,
        exit_group, quit};

template <typename T>
string serialize_object(T object){

    std::stringstream ss;
    cereal::BinaryOutputArchive oarchive(ss); // Create an output archive
    // PortableBinaryOutputArchive maybe needed
    oarchive(object); // Write the data to the archive

    return ss.str();
}

template <typename T>
T deserialize_object(string reply){

    std::stringstream ss;
    ss << reply;

    cereal::BinaryInputArchive iarchive(ss); // Create an output archive
    //std::unique_ptr<T> deserialized_message(nullptr);
    T deserialized_message;
    iarchive(deserialized_message); // read the data from the archive

    return deserialized_message;
}



//map<string, controlMessageType> message_type_map = {
//        {"!r", register_client},
//        {"!lg", list_groups}
//};

//template <typename T>


#endif
