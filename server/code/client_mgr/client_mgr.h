#ifndef CLIENT_MGR_H
#define CLIENT_MGR_H

#include <map>

#include "event.h"
#include "event_bus.h"
#include "user.h"

namespace client_mgr
{
    class ClientMgr
    {
    private:
        EventBus &event_bus;
        event_receiver::EventReceiver event_receiver;
        const time_t timeout = 10 * 60; // 10min

        std::map<user::User, time_t> user_timeouts;

        void ConnectionOpened(event::Event &event);

        void ConnectionClosed(event::Event &event);

        void NewMessage(event::Event &event);

        void GetUsers(event::Event &event);

        void EventHandler(event::Event &event);

        void Cyclic();

    public:
        ClientMgr(EventBus &event_bus, const event_receiver::eventReceiverId_t receiver_id);

        void run();
    };
} // namespace client_mgr

#endif