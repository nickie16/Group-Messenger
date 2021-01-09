#include "ClientEntry.h"


ClientEntry::ClientEntry(int id, const string &username, const string &address, int port) : id(id), username(username),
                                                                                            address(address),
                                                                                            port(port) {}

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

const string &ClientEntry::getAddress() const {
    return address;
}

void ClientEntry::setAddress(const string &address) {
    ClientEntry::address = address;
}

int ClientEntry::getPort() const {
    return port;
}

void ClientEntry::setPort(int port) {
    ClientEntry::port = port;
}
