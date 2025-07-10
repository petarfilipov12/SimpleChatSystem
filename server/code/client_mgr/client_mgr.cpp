#include "client_mgr.h"

#include <set>
#include <thread>
#include <chrono>
#include <memory>

#include "message.h"

using namespace client_mgr;

void ClientMgr::ConnectionOpened(event_bus::Event &event)
{
    std::shared_ptr<const common::User> p_user = std::static_pointer_cast<const common::User>(event.GetDataIn());

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    this->user_timeouts[*p_user] = time(nullptr);
    lock_user_timeouts.unlock();

    std::unique_lock<std::shared_mutex> lock_users(this->mtx_users);
    this->users.insert(*p_user);
}

void ClientMgr::ConnectionClosed(event_bus::Event &event)
{
    std::shared_ptr<const common::User> p_user = std::static_pointer_cast<const common::User>(event.GetDataIn());

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    this->user_timeouts.erase(*p_user);
    lock_user_timeouts.unlock();

    std::unique_lock<std::shared_mutex> lock_users(this->mtx_users);
    this->users.erase(*p_user);
}

void ClientMgr::NewMessage(event_bus::Event &event)
{
    std::shared_ptr<const common::Message> p_message = std::static_pointer_cast<const common::Message>(event.GetDataIn());

    std::unique_lock<std::mutex> lock_user_timeouts(this->mtx_user_timeouts);
    this->user_timeouts[p_message->GetUser()] = time(nullptr);
}

void ClientMgr::GetUsers(event_bus::Event &event) const
{
    std::shared_ptr<std::set<common::User> > p_users = std::static_pointer_cast<std::set<common::User> >(event.GetDataOut());

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
        auto user_timeouts_snapshot = this->user_timeouts;
        lock_user_timeouts.unlock();

        for (auto it = user_timeouts_snapshot.begin(); it != user_timeouts_snapshot.end(); it++)
        {
            if (time(nullptr) >= (it->second + this->timeout))
            {
                this->event_bus.SendAsync(event_bus::Event(event_bus::EVENT_ID_DISCONNECT_USER, std::make_shared<const common::User>(it->first), nullptr));
            }
        }

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
    this->event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_GET_USERS);
}

void ClientMgr::run()
{
    std::thread t([this]
                  { this->Cyclic(); });
    t.detach();
}