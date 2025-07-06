#include "console_app.h"

#include "connected_users.pb.h"
#include "string_trim.h"

using namespace cansole_app;

returnType_t ConsoleApp::PrintUsers()
{
    connected_users::ConnectedUsers conn_users;
    returnType_t ret = this->api.GetUsers(conn_users);

    if (ret == RET_OK)
    {
        if (conn_users.users_size() > 0)
        {
            for (auto user : conn_users.users())
            {
                time_t diff = time(nullptr) - user.joined_timestamp();
                std::string t = "Time online ";
                t += std::to_string(diff / 3600) + "h:";
                t += std::to_string((diff % (3600)) / 60) + "m:";
                t += std::to_string(diff % 60) + "s";


                std::cout << user.username() << " | ";
                std::cout << user.ip() << ":" << user.port() << " | ";
                std::cout << t << std::endl;
            }
        }
        else
        {
            ret = RET_NO_LOGGED_USERS;
        }
    }

    return ret;
}

void ConsoleApp::KickUser()
{
    std::string username;

    std::cout << "Kick Username: ";
    getline(std::cin, username);

    common::trim(username);

    if (!username.empty())
    {
        this->api.KickUser(username);
    }
}

ConsoleApp::ConsoleApp(const std::string &api_endpoint)
{
    this->api = rest_api_client::RestApiClient(api_endpoint);
}

void ConsoleApp::Run()
{
    std::string temp;
    while (true)
    {
        system("clear");

        switch (this->PrintUsers())
        {
        case RET_OK:
            std::cout << std::endl;
            this->KickUser();
            break;
        case RET_NO_LOGGED_USERS:
            std::cout << "No logged users. Press enter to refresh";
            getline(std::cin, temp);
            break;
        default:
            std::cout << "Error. Press enter to refresh";
            getline(std::cin, temp);
        }
    }
}