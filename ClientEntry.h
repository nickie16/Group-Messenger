
#ifndef GROUP_MESSENGER_CLIENTENTRY_H
#define GROUP_MESSENGER_CLIENTENTRY_H

#include <string>

using std::string;

class ClientEntry {

public:
    ClientEntry(int id, const string &username, const string &address, int port);

    int getId() const;

    void setId(int id);

    const string &getUsername() const;

    void setUsername(const string &username);

    const string &getAddress() const;

    void setAddress(const string &address);

    int getPort() const;

    void setPort(int port);

private:
    int id;
    string username;
    string address;
    int port;

};

#endif //GROUP_MESSENGER_CLIENTENTRY_H
