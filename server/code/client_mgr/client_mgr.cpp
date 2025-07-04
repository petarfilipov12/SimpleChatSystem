#include "client_mgr.h"

#include <set>
#include <thread>
#include <chrono>

#include "message.h"

using namespace client_mgr;

void ClientMgr::ConnectionOpened(event_bus::Event &event)
{
    common::User *p_user = (common::User *)event.GetDataIn();

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    std::unique_lock<std::shared_mutex> lock_users(this->mtx_users);
    this->user_timeouts[*p_user] = time(nullptr);
    this->users.insert(*p_user);
}

void ClientMgr::ConnectionClosed(event_bus::Event &event)
{
    common::User *p_user = (common::User *)event.GetDataIn();

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    std::unique_lock<std::shared_mutex> lock_users(this->mtx_users);
    this->user_timeouts.erase(*p_user);
    this->users.erase(*p_user);
}

void ClientMgr::NewMessage(event_bus::Event &event)
{
    common::Message *p_message = (common::Message *)event.GetDataIn();

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    this->user_timeouts[p_message->GetUser()] = time(nullptr);
}

void ClientMgr::GetUsers(event_bus::Event &event) const
{
    std::set<common::User> *p_users = (std::set<common::User> *)event.GetDataOut();

    std::shared_lock<std::shared_mutex> lock(this->mtx_users);
    *p_users = this->users;
}

void ClientMgr::EventHandler(event_bus::Event &event)
{
    switch (event.GetEventId())
    {
    case event_bus::EVENT_ID_CONNECTION_OPENED:
        this->ConnectionOpened(event);
        break;
    case event_bus::EVENT_ID_CONNECTION_CLOSED:
        this->ConnectionClosed(event);
        break;
    case event_bus::EVENT_ID_NEW_MESSAGE:
        this->NewMessage(event);
        break;
    case event_bus::EVENT_ID_GET_USERS:
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
        std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
        for (auto it = this->user_timeouts.begin(); it != this->user_timeouts.end(); it++)
        {
            if (time(nullptr) >= (it->second + this->timeout))
            {
                this->event_bus.Send(event_bus::Event(event_bus::EVENT_ID_DISCONNECT_USER, &(it->first), nullptr));
            }
        }
        lock_user_timeouts.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

ClientMgr::ClientMgr(event_bus::EventBus &event_bus, const event_bus::eventReceiverId_t receiver_id) : event_bus(event_bus)
{
    this->event_receiver = event_bus::EventReceiver(receiver_id, std::bind(&ClientMgr::EventHandler, this, std::placeholders::_1));

    this->event_bus.AddReceiver(this->event_receiver);
    this->event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_CONNECTION_OPENED);
    this->event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_CONNECTION_CLOSED);
    this->event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_NEW_MESSAGE);
}

void ClientMgr::run()
{
    std::thread t([this]
                  { this->Cyclic(); });
    t.detach();
}