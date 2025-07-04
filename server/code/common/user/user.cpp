#include "user.h"
#include <ctime>

using namespace common;

User::User()
{
    this->username = "";
    this->ip = "";
    this->port = 0;
}

User::User(std::string username)
{
    this->username = username;
    this->ip = "";
    this->port = 0;
}

User::User(std::string username, std::string ip, uint port)
{
    this->username = username;
    this->ip = ip;
    this->port = port;
}

std::string User::GetUsername() const
{
    return this->username;
}

std::string User::GetIp() const
{
    return this->ip;
}

uint User::GetPort() const
{
    return this->port;
}

time_t User::GetJoinedTimestamp() const
{
    return this->joined_timestamp;
}

void User::SetCurrentTime()
{
    this->joined_timestamp = time(nullptr);
}

std::string User::ToString() const
{
    return this->username + " -> " + this->ip + ":" + std::to_string(this->port);
}

User& User::operator=(const User &u)
{
    this->username = u.username;
    this->ip = u.ip;
    this->port = u.port;
    this->joined_timestamp = u.joined_timestamp;

    return *this;
}

bool User::operator==(const User &u) const
{
    bool ret = false;

    if (this->username == u.username)
    {
        ret = true;
    }

    return ret;
}

bool User::operator<(const User &u) const
{
    return this->username < u.username;
}
