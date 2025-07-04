#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <map>
#include <set>
#include <mutex>
#include <shared_mutex>

#include "return_type.h"

#include "event_bus.h"
#include "event.h"
#include "user.h"

namespace websocket_server
{
    class WebsocketServer
    {
    private:
        event_bus::EventBus &event_bus;
        event_bus::EventReceiver event_receiver;

        std::map<websocketpp::connection_hdl, common::User, std::owner_less<websocketpp::connection_hdl>> conn_user_map;
        std::set<std::string> usernames;

        std::map<websocketpp::connection_hdl, common::User, std::owner_less<websocketpp::connection_hdl>> val_map;

        std::shared_mutex mtx_conn_user_map;
        std::shared_mutex mtx_usernames;
        std::mutex mtx_val_map;

        websocketpp::server<websocketpp::config::asio> srv;
        const uint srv_port;

        void OpenHandler(websocketpp::connection_hdl hdl);

        void CloseHandler(websocketpp::connection_hdl hdl);

        void MessageHandler(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);

        bool ValidateHandler(websocketpp::connection_hdl hdl);

        void FailHandler(websocketpp::connection_hdl hdl);

        void CloseConnection(websocketpp::connection_hdl hdl);

        returnType_t GetConnectionUser(websocketpp::connection_hdl &hdl, common::User &user);

        returnType_t GetConnectionUsername(websocketpp::connection_hdl &hdl, std::string &username);

        void GetConnectionIP(websocketpp::connection_hdl &hdl, std::string &ip);

        void GetConnectionPort(websocketpp::connection_hdl &hdl, uint &port);

        void DisconnectUser(event_bus::Event &event);

        void EventHandler(event_bus::Event &event);

    public:
        WebsocketServer(event_bus::EventBus &event_bus, const uint port, event_bus::eventReceiverId_t receiver_id);

        void run();
    };
} // namespace websocket_server

#endif