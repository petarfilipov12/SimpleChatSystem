#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

struct Message
{
    const std::string m_username;
    const std::string m_text;

    std::string ToString() const
    {
        return this->m_username + ": " + this->m_text;
    }
};

#endif 