#ifndef REST_API_CLIENT_H
#define REST_API_CLIENT_H

#include <string>

#include "return_type.h"

#include <cpr/cpr.h>
#include "connected_users.pb.h"
#include "kick_user.pb.h"

namespace rest_api_client
{
    class RestApiClient
    {
    private:
        std::string api_endpoint;

        returnType_t Post(const std::string &service_path, const std::string &data, std::string &res)
        {
            returnType_t ret = RET_NOT_OK;

            cpr::Response r = cpr::Post(
                cpr::Url{this->api_endpoint + service_path},
                cpr::Body{data},
                cpr::Header{{"Content-Type", "text/plain"}});

            if (r.status_code == 200)
            {
                res = r.text;
                ret = RET_OK;
            }

            return ret;
        }

        returnType_t Post(const std::string &service_path, std::string &res)
        {
            returnType_t ret = RET_NOT_OK;

            cpr::Response r = cpr::Post(
                cpr::Url{this->api_endpoint + service_path});

            if (r.status_code == 200)
            {
                res = r.text;
                ret = RET_OK;
            }

            return ret;
        }

    public:
        RestApiClient()
        {
            this->api_endpoint = "http://127.0.0.1:8080";
        }
    
        RestApiClient(const std::string &api_endpoint) : api_endpoint(api_endpoint)
        {
        }

        returnType_t GetUsers(connected_users::ConnectedUsers &conn_users)
        {
            returnType_t ret = RET_NOT_OK;
            std::string res;

            ret = this->Post("/get_users", res);

            if (ret == RET_OK)
            {
                conn_users.ParseFromString(res);
            }

            return ret;
        }

        returnType_t KickUser(const std::string &username)
        {
            returnType_t ret = RET_NOT_OK;
            std::string res;

            kick_user::KickUser kick_usr;
            kick_usr.set_username("User1");

            ret = this->Post("/kick_user", kick_usr.SerializeAsString(), res);

            return ret;
        }
    };
} // namespace rest_api_client

#endif