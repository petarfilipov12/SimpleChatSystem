#include "client_mgr.h"

#include <set>
#include <thread>
#include <chrono>

#include "message.h"

using namespace client_mgr;

void ClientMgr::ConnectionOpened(event::Event &event)
{
    user::User *p_user = (user::User *)event.GetDataIn();
    this->user_timeouts[*p_user] = time(nullptr);
}

void ClientMgr::ConnectionClosed(event::Event &event)
{
    user::User *p_user = (user::User *)event.GetDataIn();
    this->user_timeouts.erase(*p_user);
}

void ClientMgr::NewMessage(event::Event &event)
{
    message::Message *p_message = (message::Message *)event.GetDataIn();
    this->user_timeouts[p_message->GetUser()] = time(nullptr);
}

void ClientMgr::GetUsers(event::Event &event)
{
    std::set<user::User> *p_users = (std::set<user::User> *)event.GetDataOut();

    for (auto it = this->user_timeouts.begin(); it != this->user_timeouts.end(); it++)
    {
        p_users->insert(it->first);
    }
}

void ClientMgr::EventHandler(event::Event &event)
{
    switch (event.GetEventId())
    {
    case event::EVENT_ID_CONNECTION_OPENED:
        this->ConnectionOpened(event);
        break;
    case event::EVENT_ID_CONNECTION_CLOSED:
        this->ConnectionClosed(event);
        break;
    case event::EVENT_ID_NEW_MESSAGE:
        this->NewMessage(event);
        break;
    case event::EVENT_ID_GET_USERS:
        this->GetUsers(event);
        break;
    default:
        break;
    }
}

void ClientMgr::Cyclic()
{
    while (true)
    {
        for (auto it = this->user_timeouts.begin(); it != this->user_timeouts.end(); it++)
        {
            if (time(nullptr) >= (it->second + this->timeout))
            {
                this->event_bus.Send(event::Event(event::EVENT_ID_DISCONNECT_USER, &(it->first), nullptr));
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

ClientMgr::ClientMgr(EventBus &event_bus, const event_receiver::eventReceiverId_t receiver_id) : event_bus(event_bus)
{
    this->event_receiver = event_receiver::EventReceiver(receiver_id, std::bind(&ClientMgr::EventHandler, this, std::placeholders::_1));

    this->event_bus.AddReceiver(this->event_receiver);
    this->event_bus.Subscribe(receiver_id, event::EVENT_ID_CONNECTION_OPENED);
    this->event_bus.Subscribe(receiver_id, event::EVENT_ID_CONNECTION_CLOSED);
    this->event_bus.Subscribe(receiver_id, event::EVENT_ID_NEW_MESSAGE);
}

void ClientMgr::run()
{
    std::thread t([this]
                  { this->Cyclic(); });
    t.detach();
}