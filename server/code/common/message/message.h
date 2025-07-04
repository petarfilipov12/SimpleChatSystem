#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "user.h"

namespace message
{
    class Message
    {
    private:
        const user::User user;
        const std::string text;
        const time_t msg_timestamp;

    public:
        Message(const user::User &user, const std::string &text, const time_t &msg_timestamp);

        user::User GetUser() const;

        std::string GetText() const;

        std::string ToString() const;
    };
} // namespace message

#endif