
#ifndef GROUP_MESSENGER_GROUPMESSAGE_H
#define GROUP_MESSENGER_GROUPMESSAGE_H

#include <iostream>
#include <string>
#include <cereal/archives/binary.hpp>

using std::string;

class GroupMessage {

private:
    string username;
    string message;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(username, message);
    }

public:
    GroupMessage() = default;

    GroupMessage(const string &username, const string &message);

    const string &getUsername() const;

    void setUsername(const string &username);

    const string &getMessage() const;

    void setMessage(const string &message);

};

#endif //GROUP_MESSENGER_GROUPMESSAGE_H


