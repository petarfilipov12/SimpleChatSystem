#include "websocket_server.h"

#include <thread>
#include <ctime>

#include "message.h"

using namespace websocket_server;

void WebsocketServer::OpenHandler(websocketpp::connection_hdl hdl)
{
    common::User user = this->val_map[hdl];
    this->val_map.erase(hdl);

    user.SetCurrentTime();

    this->conn_user_map.insert({hdl, user});
    this->usernames.insert(user.GetUsername());

    this->event_bus.Send(event_bus::Event(event_bus::EVENT_ID_CONNECTION_OPENED, &user, nullptr));
}

void WebsocketServer::CloseHandler(websocketpp::connection_hdl hdl)
{
    common::User user = this->conn_user_map[hdl];
    this->conn_user_map.erase(hdl);
    this->usernames.erase(user.GetUsername());

    this->event_bus.Send(event_bus::Event(event_bus::EVENT_ID_CONNECTION_CLOSED, &user, nullptr));
}

void WebsocketServer::MessageHandler(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg)
{
    common::Message message(this->conn_user_map[hdl], msg->get_payload(), time(nullptr));

    this->event_bus.Send(event_bus::Event(event_bus::EVENT_ID_NEW_MESSAGE, &message, nullptr));

    for (auto it = this->conn_user_map.begin(); it != this->conn_user_map.end(); it++)
    {
        this->srv.send(it->first, message.ToString(), msg->get_opcode());
    }
}

bool WebsocketServer::ValidateHandler(websocketpp::connection_hdl hdl)
{
    common::User user;

    bool ret = false;

    if (
        (this->GetConnectionUser(hdl, user) == RET_OK) &&
        (this->usernames.find(user.GetUsername()) == this->usernames.end()))
    {
        ret = true;
    }

    this->val_map.insert({hdl, user});

    return ret;
}

void WebsocketServer::FailHandler(websocketpp::connection_hdl hdl)
{
    common::User user = this->val_map[hdl];
    this->val_map.erase(hdl);

    this->event_bus.Send(event_bus::Event(event_bus::EVENT_ID_CONNECTION_FAILED, &user, nullptr));
}

void WebsocketServer::CloseConnection(websocketpp::connection_hdl hdl)
{
    this->srv.close(hdl, websocketpp::close::status::normal, "");
}

returnType_t WebsocketServer::GetConnectionUser(websocketpp::connection_hdl &hdl, common::User &user)
{
    returnType_t ret = RET_NOT_OK;

    std::string username;
    std::string ip;
    uint port;

    ret = this->GetConnectionUsername(hdl, username);
    this->GetConnectionIP(hdl, ip);
    this->GetConnectionPort(hdl, port);

    user = common::User(username, ip, port);

    return ret;
}

returnType_t WebsocketServer::GetConnectionUsername(websocketpp::connection_hdl &hdl, std::string &username)
{
    returnType_t ret = RET_OK;

    username = this->srv.get_con_from_hdl(hdl)->get_request_header("Username");

    if (username.empty())
    {
        ret = RET_USERNAME_HEADER_MISSING;
    }

    return ret;
}

void WebsocketServer::GetConnectionIP(websocketpp::connection_hdl &hdl, std::string &ip)
{
    const auto addr = this->srv.get_con_from_hdl(hdl)->get_raw_socket().remote_endpoint().address();

    if (addr.is_v6() && addr.to_v6().is_v4_mapped())
    {
        ip = addr.to_v6().to_v4().to_string();
    }
    else
    {
        ip = addr.to_string();
    }
}

void WebsocketServer::GetConnectionPort(websocketpp::connection_hdl &hdl, uint &port)
{
    port = this->srv.get_con_from_hdl(hdl)->get_raw_socket().remote_endpoint().port();
}

void WebsocketServer::DisconnectUser(event_bus::Event &event)
{
    common::User *p_user = (common::User *)event.GetDataIn();

    for (auto it = this->conn_user_map.begin(); it != this->conn_user_map.end(); it++)
    {
        if (it->second == *p_user)
        {
            this->CloseConnection(it->first);
            break;
        }
    }
}

void WebsocketServer::EventHandler(event_bus::Event &event)
{
    switch (event.GetEventId())
    {
    case event_bus::EVENT_ID_DISCONNECT_USER:
        this->DisconnectUser(event);
        break;
    default:
        break;
    }
}

WebsocketServer::WebsocketServer(event_bus::EventBus &event_bus, const uint port, event_bus::eventReceiverId_t receiver_id) : event_bus(event_bus), srv_port(port)
{
    this->srv.init_asio();

    this->srv.set_open_handler(websocketpp::lib::bind(&WebsocketServer::OpenHandler, this, websocketpp::lib::placeholders::_1));
    this->srv.set_close_handler(websocketpp::lib::bind(&WebsocketServer::CloseHandler, this, websocketpp::lib::placeholders::_1));
    this->srv.set_message_handler(websocketpp::lib::bind(&WebsocketServer::MessageHandler, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
    this->srv.set_validate_handler(websocketpp::lib::bind(&WebsocketServer::ValidateHandler, this, websocketpp::lib::placeholders::_1));
    this->srv.set_fail_handler(websocketpp::lib::bind(&WebsocketServer::FailHandler, this, websocketpp::lib::placeholders::_1));

    this->event_receiver = event_bus::EventReceiver(receiver_id, std::bind(&WebsocketServer::EventHandler, this, std::placeholders::_1));

    this->event_bus.AddReceiver(this->event_receiver);
    this->event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_DISCONNECT_USER);
}

void WebsocketServer::run()
{
    this->srv.listen(this->srv_port);
    this->srv.start_accept();

    std::thread t([this]{ this->srv.run(); });
    t.detach();
}