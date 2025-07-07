#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "return_type.h"

#include <unordered_map>
#include <set>
#include <shared_mutex>
#include <mutex>
#include <queue>

#include "event.h"
#include "event_receiver.h"

namespace event_bus
{
    class EventBus
    {
    private:
        std::unordered_map<eventReceiverId_t, EventReceiver*> event_receivers;
        std::unordered_map<eventId_t, std::set<eventReceiverId_t>> event_to_receivers_map;
        std::queue<EventAsync> event_async_queue;

        std::shared_mutex mtx_event_receivers;
        std::shared_mutex mtx_event_to_receivers_map;
        std::mutex event_async_queue_lock;

        void EmitAsync(EventAsync event_async);
        void Cyclic();

    public:

        returnType_t AddReceiver(EventReceiver &event_receiver);

        returnType_t RemoveReceiver(const eventReceiverId_t receiver_id);

        returnType_t Subscribe(const eventReceiverId_t receiver_id, const eventId_t event_id);

        returnType_t Unsubscribe(const eventReceiverId_t receiver_id, const eventId_t event_id);

        returnType_t SendSync(const Event &event);

        void SendAsync(const EventAsync &event_async);

        void run();
    };
} // namespace event_bus

#endif