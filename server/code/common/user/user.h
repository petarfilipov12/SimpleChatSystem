#ifndef USER_H
#define USER_H

#include <string>
#include <ctime>

class User
{
private:
    std::string username;
    std::string ip;
    uint port;
    time_t joined_timestamp;

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

    time_t GetJoinedTimestamp() const
    {
        return this->joined_timestamp;
    }

    void SetCurrentTime()
    {
        this->joined_timestamp = time(nullptr);
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
        this->joined_timestamp = u.joined_timestamp;

        return *this;
    }

    bool operator==(const User& u) const
    {
        bool ret = false;

        if(this->username == u.username)
        {
            ret = true;
        }

        return ret;
    }

    bool operator<(const User& u) const
    {
        return this->username < u.username;
    }
};

#endif 