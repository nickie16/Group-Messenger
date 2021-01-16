
#ifndef GROUP_MESSENGER_CLIENTENTRY_H
#define GROUP_MESSENGER_CLIENTENTRY_H

#include <string>
#include "common.h"

using std::string;

class ClientEntry {

private:
    int id;
    string username;
    string ip;
    int port;

    friend class cereal::access;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(id, username, ip, port);
    }


public:
    ClientEntry() = default;

    ClientEntry(int id, const string &username, const string &ip, int port);

    int getId() const;

    void setId(int id);

    const string &getUsername() const;

    void setUsername(const string &username);

    const string &getIp() const;

    void setIp(const string &address);

    int getPort() const;

    void setPort(int port);

};

#endif //GROUP_MESSENGER_CLIENTENTRY_H
