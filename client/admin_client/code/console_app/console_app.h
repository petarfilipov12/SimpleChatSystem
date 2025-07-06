#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include <string>

#include "return_type.h"
#include "rest_api_client.h"


namespace cansole_app
{
    class ConsoleApp
    {
    private:
        rest_api_client::RestApiClient api;

        returnType_t PrintUsers();

        void KickUser();

    public:
        ConsoleApp(const std::string &api_endpoint);

        void Run();
    };
} // namespace cansole_app

#endif