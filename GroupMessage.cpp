
#include "GroupMessage.h"


GroupMessage::GroupMessage(const string &username, const string &message) : username(username), message(message) {}

const string &GroupMessage::getUsername() const {
    return username;
}

void GroupMessage::setUsername(const string &username) {
    GroupMessage::username = username;
}

const string &GroupMessage::getMessage() const {
    return message;
}

void GroupMessage::setMessage(const string &message) {
    GroupMessage::message = message;
}
