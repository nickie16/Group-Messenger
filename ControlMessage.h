
#ifndef GROUP_MESSENGER_CONTROLMESSAGE_H
#define GROUP_MESSENGER_CONTROLMESSAGE_H

#include <iostream>
#include <string>
#include "common.h"

using std::string;

class ControlMessage{

private:
    int id_user;
    string username;
    controlMessageType messageType;
    string params;

public:
    ControlMessage(int idUser, const string &username, controlMessageType messageType, const string &params);

    const string &getParams() const;

    void setParams(const string &params);

    int getId() const;

    void setId(int id);

    const string &getUsername() const;

    void setUsername(const string &username);

    controlMessageType getMessageType() const;

    void setMessageType(controlMessageType messageType);

};

#endif //GROUP_MESSENGER_CONTROLMESSAGE_H
