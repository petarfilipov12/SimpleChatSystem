#include "message.h"

#include <ctime>

using namespace common;

Message::Message(const User &user, const std::string &text, const time_t &msg_timestamp) : user(user), text(text), msg_timestamp(msg_timestamp)
{
}

const User& Message::GetUser() const
{
    return this->user;
}

const std::string& Message::GetText() const
{
    return this->text;
}

std::string Message::GetDate() const
{
    const struct tm* const date = localtime(&(this->msg_timestamp));

    std::string d = ("[" +
                     std::to_string(1900 + date->tm_year) + "-" +
                     std::to_string(date->tm_mon) + "-" +
                     std::to_string(date->tm_mday) +
                     " " +
                     std::to_string(date->tm_hour) + ":" +
                     std::to_string(date->tm_min) + ":" +
                     std::to_string(date->tm_sec) +
                     "]");
    
    return d;
}

std::string Message::ToString() const
{
    std::string d = this->GetDate();

    return d + " " + this->user.GetUsername() + ": " + this->text;
}
