#include "rest_server.h"

#include "return_type.h"

#include <set>

#include "event.h"
#include "user.h"

#include "kick_user.pb.h"
#include "connected_users.pb.h"

void RestServer::InitServices()
{
    this->Post("/get_users", std::bind(&RestServer::Handler_GetUsers, this, std::placeholders::_1, std::placeholders::_2));
    this->Post("/kick_user", std::bind(&RestServer::Handler_KickUser, this, std::placeholders::_1, std::placeholders::_2));
}

RestServer::RestServer(EventBus& event_bus): event_bus(event_bus)
{
    this->host = "0.0.0.0";
    this->port = 8080;
}

RestServer::RestServer(const std::string& host, const unsigned int port, EventBus& event_bus): event_bus(event_bus)
{
    this->host = host;
    this->port = port;
}

RestServer::~RestServer()
{
    delete this->svr;
}

void RestServer::Post(const std::string& url_path, const std::function<void(const httplib::Request &, httplib::Response &)> handler_func)
{
    this->svr->Post(url_path, handler_func);
}

void RestServer::run()
{
    std::thread thread_rest_server([this]{this->svr->listen(this->host, this->port);});
    thread_rest_server.detach();
}

/************************/
/*Handler Implementation*/
/************************/
void RestServer::Handler_GetUsers(const httplib::Request &req, httplib::Response &res)
{   
    std::set<User> users;
    this->event_bus.Send(Event(EVENT_ID_DISCONNECT_USER, nullptr, &users));

    connected_users::ConnectedUsers conn_users_msg;

    for(auto it = users.begin(); it != users.end(); it++)
    {
        auto user_msg = conn_users_msg.add_users();
        user_msg->set_username(it->GetUsername());
        user_msg->set_ip(it->GetIp());
        user_msg->set_port(it->GetPort());
        user_msg->set_joined_timestamp(it->GetJoinedTimestamp());
    }

    res.set_content(conn_users_msg.SerializeAsString(), "application/json");
}

void RestServer::Handler_KickUser(const httplib::Request &req, httplib::Response &res)
{
    kick_user::KickUser kick_user_msg;
    kick_user_msg.ParseFromString(req.body);
    
    User user(kick_user_msg.username());

    this->event_bus.Send(Event(EVENT_ID_DISCONNECT_USER, &user, nullptr));

    res.set_content("OK", "application/json");
}
