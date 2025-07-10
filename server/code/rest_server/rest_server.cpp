#include "rest_server.h"

#include <set>

#include "event.h"
#include "user.h"

#include "kick_user.pb.h"
#include "connected_users.pb.h"

using namespace rest_server;

void RestServer::InitServices()
{
    this->Post("/get_users", std::bind(&RestServer::Handler_GetUsers, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/kick_user", std::bind(&RestServer::Handler_KickUser, this, std::placeholders::_1, std::placeholders::_2));
}

RestServer::RestServer(event_bus::EventBus& event_bus): event_bus(event_bus)
{
    this->host = "0.0.0.0";
    this->port = 8080;

    this->InitServices();
}

RestServer::RestServer(const std::string& host, const unsigned int port, event_bus::EventBus& event_bus): event_bus(event_bus)
{
    this->host = host;
    this->port = port;

    this->InitServices();
}

void RestServer::Post(const std::string& url_path, const std::function<void(const httplib::Request &, httplib::Response &)> handler_func)
{
    this->svr.Post(url_path, handler_func);
}

void RestServer::run()
{
    std::thread thread_rest_server([this]{this->svr.listen(this->host, this->port);});
    thread_rest_server.detach();
}

void RestServer::Handler_GetUsers(const httplib::Request &req, httplib::Response &res)
{   
    std::shared_ptr<std::set<common::User> > p_users = std::make_shared<std::set<common::User> >(std::set<common::User>());
    this->event_bus.SendSync(event_bus::Event(event_bus::EVENT_ID_GET_USERS, nullptr, p_users));

    connected_users::ConnectedUsers conn_users_msg;

    for(auto it = p_users->begin(); it != p_users->end(); it++)
    {
        auto user_msg = conn_users_msg.add_users();
        user_msg->set_username(it->GetUsername());
        user_msg->set_ip(it->GetIp());
        user_msg->set_port(it->GetPort());
        user_msg->set_joined_timestamp(it->GetJoinedTimestamp());
    }

    res.set_content(conn_users_msg.SerializeAsString(), "text/plain");
}

void RestServer::Handler_KickUser(const httplib::Request &req, httplib::Response &res)
{
    kick_user::KickUser kick_user_msg;
    kick_user_msg.ParseFromString(req.body);
    
    std::shared_ptr<const common::User> p_user = std::make_shared<const common::User>(common::User(kick_user_msg.username()));

    this->event_bus.SendAsync(event_bus::Event(event_bus::EVENT_ID_DISCONNECT_USER, p_user, nullptr));

    res.set_content("OK", "text/plain");
}
