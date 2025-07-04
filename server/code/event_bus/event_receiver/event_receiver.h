#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include "return_type.h"

#include <set>
#include <functional>

#include "event.h"

#define RECEIVER_ID_INVALID 0xffffu;

namespace event_bus
{
    typedef int eventReceiverId_t;

    class EventReceiver
    {
    private:
        eventReceiverId_t id;
        std::set<eventId_t> events;
        std::function<void(Event &)> callback;

    public:
        EventReceiver();

        EventReceiver(const eventReceiverId_t id, const std::function<void(event_bus::Event &)> callback);

        ~EventReceiver();

        returnType_t AddEvent(const eventId_t event_id);

        returnType_t RemoveEvent(const eventId_t event_id);

        bool IsEmpty() const;

        bool ContainsEvent(const eventId_t event_id) const;

        std::set<eventId_t> *GetEvents();

        eventReceiverId_t GetId() const;

        std::function<void(Event &)> GetCallback() const;
    };
} // namespace event_receiver

#endif