#include "ClientEntry.h"


ClientEntry::ClientEntry(int id, const string &username, const string &ip, int port) : id(id), username(username),
                                                                                            ip(ip), port(port) {}

int ClientEntry::getId() const {
    return id;
}

void ClientEntry::setId(int id) {
    ClientEntry::id = id;
}

const string &ClientEntry::getUsername() const {
    return username;
}

void ClientEntry::setUsername(const string &username) {
    ClientEntry::username = username;
}

const string &ClientEntry::getIp() const {
    return ip;
}

void ClientEntry::setIp(const string &address) {
    ClientEntry::ip = address;
}

int ClientEntry::getPort() const {
    return port;
}

void ClientEntry::setPort(int port) {
    ClientEntry::port = port;
}
