#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include <string>

#include "return_type.h"
#include "rest_api_client.h"

#include "connected_users.pb.h"

namespace cansole_app
{
    class ConsoleApp
    {
    private:
        rest_api_client::RestApiClient api;

        returnType_t PrintUsers()
        {
            connected_users::ConnectedUsers conn_users;
            returnType_t ret = this->api.GetUsers(conn_users);

            if (ret == RET_OK)
            {
                if(conn_users.users_size() > 0)
                {
                    for (auto user : conn_users.users())
                    {
                        std::cout << user.username() << " | ";
                        std::cout << user.ip() << ":" << user.port() << " | ";
                        std::cout << user.joined_timestamp() << std::endl;
                    }
                }
                else
                {
                    ret = RET_NO_LOGGED_USERS;
                }
            }

            return ret;
        }

        void KickUser()
        {
            std::string username;

            std::cout << "Kick Username: ";
            getline(std::cin, username);

            if (!username.empty())
            {
                this->api.KickUser(username);
            }
        }

    public:
        ConsoleApp(const std::string &api_endpoint)
        {
            this->api = rest_api_client::RestApiClient(api_endpoint);
        }

        void Run()
        {
            std::string temp;
            while (true)
            {
                system("clear");
                
                switch(this->PrintUsers())
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
    };
} // namespace cansole_app

#endif