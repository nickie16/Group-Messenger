
#ifndef GROUP_MESSENGER_CONTROLMESSAGE_H
#define GROUP_MESSENGER_CONTROLMESSAGE_H

#include <iostream>
#include <string>
#include <sstream>
#include "common.h"

using std::string;

class ControlMessage{

private:
    int id_user{};
    string username;
    string messageType;
    string params;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive( id_user, username, messageType, params );
    }

public:
    ControlMessage(int idUser, const string username, string messageType, const string  params);

    ControlMessage() = default;

    const string &getParams() const;

    void setParams(const string &params);

    int getId() const;

    void setId(int id);

    const string &getUsername() const;

    void setUsername(const string &username);

    string getMessageType() const;

    void setMessageType(string messageType);

//    template <class Archive>
//    static void load_and_construct( Archive & ar, cereal::construct<ControlMessage> & construct )
//    {
//        int id_user;
//        string username;
//        string messageType;
//        string params;
//        ar( id_user, username, messageType,  params);
//        construct( id_user, username, messageType,  params ); // calls MyType( x )
//    }

};

#endif //GROUP_MESSENGER_CONTROLMESSAGE_H
