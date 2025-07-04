#ifndef CLIENT_MGR_H
#define CLIENT_MGR_H

#include <map>
#include <set>
#include <shared_mutex>
#include <mutex>

#include "event.h"
#include "event_bus.h"
#include "user.h"

namespace client_mgr
{
    class ClientMgr
    {
    private:
        event_bus::EventBus &event_bus;
        event_bus::EventReceiver event_receiver;
        const time_t timeout = 10 * 60; // 10min

        std::map<common::User, time_t> user_timeouts;
        std::set<common::User> users;

        std::mutex mtx_user_timeouts;
        mutable std::shared_mutex mtx_users;

        void ConnectionOpened(event_bus::Event &event);

        void ConnectionClosed(event_bus::Event &event);

        void NewMessage(event_bus::Event &event);

        void GetUsers(event_bus::Event &event) const;

        void EventHandler(event_bus::Event &event);

        void Cyclic();

    public:
        ClientMgr(event_bus::EventBus &event_bus, const event_bus::eventReceiverId_t receiver_id);

        void run();
    };
} // namespace client_mgr

#endif