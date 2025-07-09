#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "user.h"

namespace common
{
    class Message
    {
    private:
        const User user;
        const std::string text;
        const time_t msg_timestamp;

    public:
        Message(const User &user, const std::string &text, const time_t &msg_timestamp);

        const User& GetUser() const;

        const std::string& GetText() const;

        std::string GetDate() const;

        std::string ToString() const;
    };
} // namespace common

#endif