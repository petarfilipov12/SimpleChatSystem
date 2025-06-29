#ifndef USER_H
#define USER_H

#include <string>

class User
{
private:
    std::string username;
    std::string ip;
    uint port;

public:
    User()
    {
        this->username = "";
        this->ip = "";
        this->port = 0;
    }

    User(std::string username, std::string ip, uint port)
    {
        this->username = username;
        this->ip = ip;
        this->port = port;
    }

    std::string GetUsername() const
    {
        return this->username;
    }

    std::string GetIp() const
    {
        return this->ip;
    }

    uint GetPort() const
    {
        return this->port;
    }

    std::string ToString() const
    {
        return this->username + " -> " + this->ip + ":" + std::to_string(this->port);
    }

    User& operator=(const User& u)
    {
        this->username = u.username;
        this->ip = u.ip;
        this->port = u.port;
    }
};

#endif 