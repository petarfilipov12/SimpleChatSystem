#ifndef EVENT_RECEIVER_H
#define EVENT_RECEIVER_H

#include "return_type.h"

#include <set>
#include <functional>

#include "event.h"

typedef int eventReceiverId_t;
#define RECEIVER_ID_INVALID 0xffffu;

class EventReceiver {
    private:
        eventReceiverId_t id;
        std::set<eventId_t> events;
        std::function<void(Event)> callback;

    public:
        EventReceiver()
        {
            this->id = RECEIVER_ID_INVALID;
            this->callback = nullptr;
        }

        EventReceiver(const eventReceiverId_t id, const std::function<void(Event)> callback)
        {
            this->id = id;
            this->callback = callback;
        }

        //~EventReceiver(){}

        returnType_t AddEvent(const eventId_t event_id)
        {
            returnType_t ret = RET_EVENT_ID_INVALID;

            if(event_id < EVENT_ID_INVALID)
            {
                this->events.insert(event_id);

                ret = RET_OK;
            }

            return ret;
        }

        returnType_t RemoveEvent(const eventId_t event_id)
        {
            returnType_t ret = RET_EVENT_ID_INVALID;

            if(event_id < EVENT_ID_INVALID)
            {
                this->events.erase(event_id);
            }

            return ret;
        }

        bool IsEmpty() const
        {
            return this->events.empty();
        }

        bool ContainsEvent(const eventId_t event_id) const
        {
            return this->events.find(event_id) != this->events.end();
        }

        std::set<eventId_t>* GetEvents()
        {
            return &this->events;
        }

        eventReceiverId_t GetId() const
        {
            return this->id;
        }

        std::function<void(Event)> GetCallback() const
        {
            return this->callback;
        }
};

#endif