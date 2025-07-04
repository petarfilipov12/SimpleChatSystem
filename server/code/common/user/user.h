#ifndef USER_H
#define USER_H

#include <string>

namespace user
{
    class User
    {
    private:
        std::string username;
        std::string ip;
        uint port;
        time_t joined_timestamp;

    public:
        User();

        User(std::string username);

        User(std::string username, std::string ip, uint port);

        std::string GetUsername() const;

        std::string GetIp() const;

        uint GetPort() const;

        time_t GetJoinedTimestamp() const;

        void SetCurrentTime();

        std::string ToString() const;

        User &operator=(const User &u);

        bool operator==(const User &u) const;

        bool operator<(const User &u) const;
    };
} // namespace user

#endif