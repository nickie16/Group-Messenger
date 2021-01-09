#include "ControlMessage.h"

// constructor using initializer list
ControlMessage::ControlMessage(int idUser, const string &username, controlMessageType messageType, const string &params)
        : id_user(idUser), username(username), messageType(messageType), params(params) {}

const string &ControlMessage::getUsername() const {
    return username;
}

void ControlMessage::setUsername(const string &username) {
    ControlMessage::username = username;
}

controlMessageType ControlMessage::getMessageType() const {
    return messageType;
}

void ControlMessage::setMessageType(controlMessageType messageType) {
    ControlMessage::messageType = messageType;
}

const string &ControlMessage::getParams() const {
    return params;
}

void ControlMessage::setParams(const string &params) {
    ControlMessage::params = params;
}

int ControlMessage::getId() const {
    return id_user;
}

void ControlMessage::setId(int id) {
    ControlMessage::id_user = id;
}
