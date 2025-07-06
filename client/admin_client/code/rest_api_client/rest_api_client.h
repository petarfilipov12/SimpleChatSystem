#ifndef REST_API_CLIENT_H
#define REST_API_CLIENT_H

#include <string>

#include "return_type.h"
#include "connected_users.pb.h"


namespace rest_api_client
{
    class RestApiClient
    {
    private:
        std::string api_endpoint;

        returnType_t Post(const std::string &service_path, const std::string &data, std::string &res);

        returnType_t Post(const std::string &service_path, std::string &res);

    public:
        RestApiClient();
    
        RestApiClient(const std::string &api_endpoint);

        returnType_t GetUsers(connected_users::ConnectedUsers &conn_users);

        returnType_t KickUser(const std::string &username);
    };
} // namespace rest_api_client

#endif