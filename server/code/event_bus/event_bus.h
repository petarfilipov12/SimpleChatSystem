#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "return_type.h"

#include <unordered_map>
#include <set>
#include <thread>
#include <vector>

#include "event.h"
#include "event_receiver.h"

class EventBus{
    private:
        std::unordered_map<eventReceiverId_t, EventReceiver> event_receivers;
        std::unordered_map<eventId_t, std::set<eventReceiverId_t> > event_to_receivers_map;

    public:
        EventBus();
        ~EventBus();

        returnType_t AddReceiver(EventReceiver& event_receiver);

        returnType_t RemoveReceiver(const eventReceiverId_t receiver_id);

        returnType_t Subscribe(const eventReceiverId_t receiver_id, const eventId_t event_id);

        returnType_t Unsubscribe(const eventReceiverId_t receiver_id, const eventId_t event_id);

        returnType_t Send(const Event& event);
};

#endif