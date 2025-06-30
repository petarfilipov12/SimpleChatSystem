#ifndef WEBSOCKET_SERVER_IMPL
#define WEBSOCKET_SERVER_IMPL

#include "return_type.h"

#include <map>
#include <set>
#include <thread>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "event_bus.h"
#include "event.h"
#include "user.h"
#include "message.h"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl connection_hdl;

namespace  placeholders = websocketpp::lib::placeholders;

using websocketpp::lib::bind;

class WebsocketBroadcastServer {
private:
    EventBus& event_bus;
    EventReceiver event_receiver;

    std::map<connection_hdl, User, std::owner_less<connection_hdl> > conn_user_map;
    std::set<std::string> usernames;

    std::map<connection_hdl, User, std::owner_less<connection_hdl> > val_map;
    
    server srv;
    const uint srv_port;

    void OpenHandler(connection_hdl hdl) {
        User user = this->val_map[hdl];
        this->val_map.erase(hdl);

        user.SetCurrentTime();

        this->conn_user_map.insert({hdl, user});
        this->usernames.insert(user.GetUsername());

        this->event_bus.Send(Event(EVENT_ID_CONNECTION_OPENED, &user, nullptr));
    }

    void CloseHandler(connection_hdl hdl) {
        User user = this->conn_user_map[hdl];
        this->conn_user_map.erase(hdl);
        this->usernames.erase(user.GetUsername());

        this->event_bus.Send(Event(EVENT_ID_CONNECTION_CLOSED, &user, nullptr));
    }

    void MessageHandler(connection_hdl hdl, server::message_ptr msg) {
        Message message(this->conn_user_map[hdl], msg->get_payload());

        this->event_bus.Send(Event(EVENT_ID_NEW_MESSAGE, &message, nullptr));

        for(auto it = this->conn_user_map.begin(); it != this->conn_user_map.end(); it++)
        {
            this->srv.send(it->first, message.ToString(), msg->get_opcode());
        }
    }

    bool ValidateHandler(connection_hdl hdl)
    {
        User user;

        bool ret = false;

        if(
            (this->GetConnectionUser(hdl, user) == RET_OK) &&
            (this->usernames.find(user.GetUsername()) == this->usernames.end())
        )
        {
            ret = true;
        }

        this->val_map.insert({hdl, user});

        return ret;
    }

    void FailHandler(connection_hdl hdl) {
        User user = this->val_map[hdl];
        this->val_map.erase(hdl);

        this->event_bus.Send(Event(EVENT_ID_CONNECTION_FAILED, &user, nullptr));
    }

    void CloseConnection(connection_hdl hdl)
    {
        this->srv.close(hdl, websocketpp::close::status::normal, "");
    }

    returnType_t GetConnectionUser(connection_hdl& hdl, User& user)
    {
        returnType_t ret = RET_NOT_OK;

        std::string username;
        std::string ip;
        uint port;
        
        ret = this->GetConnectionUsername(hdl, username);
        this->GetConnectionIP(hdl, ip);
        this->GetConnectionPort(hdl, port);

        user = User(username, ip, port);

        return ret;
    }

    returnType_t GetConnectionUsername(connection_hdl& hdl, std::string& username)
    {
        returnType_t ret = RET_OK;

        username = this->srv.get_con_from_hdl(hdl)->get_request_header("Username");

        if(username.empty())
        {
            ret = RET_USERNAME_HEADER_MISSING;
        }

        return ret;
    }

    void GetConnectionIP(connection_hdl& hdl, std::string& ip)
    {
        const auto addr = this->srv.get_con_from_hdl(hdl)->get_raw_socket().remote_endpoint().address();
        
        if(addr.is_v6() && addr.to_v6().is_v4_mapped())
        {
            ip = addr.to_v6().to_v4().to_string();
        }
        else
        {
            ip = addr.to_string();
        }
    }

    void GetConnectionPort(connection_hdl& hdl, uint& port)
    {
        port = this->srv.get_con_from_hdl(hdl)->get_raw_socket().remote_endpoint().port();
    }

    void DisconnectUser(Event &event)
    {
        User *p_user = (User *)event.GetDataIn();

        for (auto it = this->conn_user_map.begin(); it != this->conn_user_map.end(); it++)
        {
            if (it->second == *p_user)
            {
                this->CloseConnection(it->first);
                break;
            }
        }
    }

    void EventHandler(Event& event)
    {
        switch(event.GetEventId())
        {
            case EVENT_ID_DISCONNECT_USER:
                this->DisconnectUser(event);
                break;
            default:
                break;
        }
    }

public:
    WebsocketBroadcastServer(EventBus& event_bus, const uint port, eventReceiverId_t receiver_id): event_bus(event_bus), srv_port(port) {
        this->srv.init_asio();

        this->srv.set_open_handler(bind(&WebsocketBroadcastServer::OpenHandler, this, placeholders::_1));
        this->srv.set_close_handler(bind(&WebsocketBroadcastServer::CloseHandler, this, placeholders::_1));
        this->srv.set_message_handler(bind(&WebsocketBroadcastServer::MessageHandler, this, placeholders::_1, placeholders::_2));
        this->srv.set_validate_handler(bind(&WebsocketBroadcastServer::ValidateHandler, this, placeholders::_1));
        this->srv.set_fail_handler(bind(&WebsocketBroadcastServer::FailHandler, this, placeholders::_1));

        this->event_receiver = EventReceiver(receiver_id, std::bind(&WebsocketBroadcastServer::EventHandler, this, std::placeholders::_1));

        this->event_bus.AddReceiver(this->event_receiver);
        this->event_bus.Subscribe(receiver_id, EVENT_ID_DISCONNECT_USER);
    }

    void run() {
        this->srv.listen(this->srv_port);
        this->srv.start_accept();
       
        std::thread t([this]{this->srv.run();});
        t.detach();
    }
    
};

#endif