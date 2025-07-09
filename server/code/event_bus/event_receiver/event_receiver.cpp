#include "event_receiver.h"

#include <mutex>

using namespace event_bus;

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
returnType_t EventReceiver::AddEvent(const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        std::unique_lock<std::shared_mutex> lock_events(this->mtx_events);
        this->events.insert(event_id);
        lock_events.unlock();

        ret = RET_OK;
    }

    return ret;
}

returnType_t EventReceiver::RemoveEvent(const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        std::unique_lock<std::shared_mutex> lock_events(this->mtx_events);
        this->events.erase(event_id);
        lock_events.unlock();

        ret = RET_OK;
    }

    return ret;
}

bool EventReceiver::IsEmpty() const
{
    std::shared_lock<std::shared_mutex> lock_events(this->mtx_events);
    return this->events.empty();
}

bool EventReceiver::ContainsEvent(const eventId_t event_id) const
{
    std::shared_lock<std::shared_mutex> lock_events(this->mtx_events);
    return this->events.find(event_id) != this->events.end();
}

const std::set<eventId_t> EventReceiver::GetEvents() const
{
    std::shared_lock<std::shared_mutex> lock_events(this->mtx_events);
    return this->events;
}

eventReceiverId_t EventReceiver::GetId() const
{
    return this->id;
}

std::function<void(Event &)> EventReceiver::GetCallback() const
{
    return this->callback;
}

EventReceiver& EventReceiver::operator=(const EventReceiver& ev_r)
{
    this->id = ev_r.id;
    this->callback = ev_r.callback;

    std::lock_guard<std::shared_mutex> lock_l(this->mtx_events);
    std::lock_guard<std::shared_mutex> lock_r(ev_r.mtx_events);
    this->events = ev_r.events;
    return *this;
}
