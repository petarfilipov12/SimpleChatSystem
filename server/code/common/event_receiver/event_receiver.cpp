#include "event_receiver.h"

EventReceiver::EventReceiver()
{
    this->id = RECEIVER_ID_INVALID;
    this->callback = nullptr;
}

EventReceiver::EventReceiver(const eventReceiverId_t id, const std::function<void(Event &)> callback)
{
    this->id = id;
    this->callback = callback;
}

EventReceiver::~EventReceiver(){}

returnType_t EventReceiver::AddEvent(const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        this->events.insert(event_id);

        ret = RET_OK;
    }

    return ret;
}

returnType_t EventReceiver::RemoveEvent(const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        this->events.erase(event_id);
    }

    return ret;
}

bool EventReceiver::IsEmpty() const
{
    return this->events.empty();
}

bool EventReceiver::ContainsEvent(const eventId_t event_id) const
{
    return this->events.find(event_id) != this->events.end();
}

std::set<eventId_t>* EventReceiver::GetEvents()
{
    return &this->events;
}

eventReceiverId_t EventReceiver::GetId() const
{
    return this->id;
}

std::function<void(Event &)> EventReceiver::GetCallback() const
{
    return this->callback;
}