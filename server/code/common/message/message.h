#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "user.h"

class Message
{
private:
    const User user;
    const std::string text;

public:
    Message(const User& user, const std::string& text): user(user), text(text)
    {

    }

    User GetUser() const
    {
        return this->user;
    }

    std::string GetText() const
    {
        return this->text;
    }

    std::string ToString() const
    {
        return this->user.GetUsername() + ": " + this->text;
    }
};

#endif 