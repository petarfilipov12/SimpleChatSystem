#ifndef USER_H
#define USER_H

#include <string>

namespace common
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

        User(const std::string& username);

        User(const std::string& username, const std::string& ip, const uint port);

        const std::string& GetUsername() const;

        const std::string& GetIp() const;

        uint GetPort() const;

        time_t GetJoinedTimestamp() const;

        void SetCurrentTime();

        std::string ToString() const;

        User &operator=(const User &u);

        bool operator==(const User &u) const;

        bool operator<(const User &u) const;
    };
} // namespace common

#endif