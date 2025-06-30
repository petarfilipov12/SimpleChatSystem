#include "event_bus.h"

EventBus::EventBus() {}
EventBus::~EventBus() {}

returnType_t EventBus::AddReceiver(EventReceiver &event_receiver)
{
    eventReceiverId_t receiver_id = event_receiver.GetId();
    returnType_t ret = RET_RECEIVER_EXISTS;

    if (this->event_receivers.find(receiver_id) == this->event_receivers.end())
    {
        this->event_receivers[receiver_id] = event_receiver;

        ret = RET_OK;
    }

    return ret;
}

returnType_t EventBus::RemoveReceiver(const eventReceiverId_t receiver_id)
{
    returnType_t ret = RET_RECEIVER_NOT_EXISTS;

    if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
    {
        std::set<eventId_t> *pEvents = this->event_receivers[receiver_id].GetEvents();
        for (std::set<eventId_t>::iterator itr_event_id = pEvents->begin(); itr_event_id != pEvents->end(); itr_event_id++)
        {
            if (this->event_to_receivers_map[*itr_event_id].erase(receiver_id))
            {
                if (this->event_to_receivers_map[*itr_event_id].empty())
                {
                    this->event_to_receivers_map.erase(*itr_event_id);
                }
            }
        }

        this->event_receivers.erase(receiver_id);

        ret = RET_OK;
    }

    return ret;
}

returnType_t EventBus::Subscribe(const eventReceiverId_t receiver_id, const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id].AddEvent(event_id);
            this->event_to_receivers_map[event_id].insert(receiver_id);

            ret = RET_OK;
        }
        else
        {
            ret = RET_RECEIVER_NOT_EXISTS;
        }
    }

    return ret;
}

returnType_t EventBus::Unsubscribe(const eventReceiverId_t receiver_id, const eventId_t event_id)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (event_id < EVENT_ID_INVALID)
    {
        if (this->event_receivers.find(receiver_id) != this->event_receivers.end())
        {
            this->event_receivers[receiver_id].RemoveEvent(event_id);

            if (this->event_to_receivers_map[event_id].erase(receiver_id))
            {
                if (this->event_to_receivers_map[event_id].empty())
                {
                    this->event_to_receivers_map.erase(event_id);
                }
            }

            ret = RET_OK;
        }
        else
        {
            ret = RET_RECEIVER_NOT_EXISTS;
        }
    }

    return ret;
}

returnType_t EventBus::Send(const Event &event)
{
    returnType_t ret = RET_EVENT_ID_INVALID;

    if (EVENT_ID_INVALID != event.GetEventId())
    {
        if (this->event_to_receivers_map.find(event.GetEventId()) != this->event_to_receivers_map.end())
        {
            std::vector<std::thread> threads;

            for (
                auto itr_receiver = this->event_to_receivers_map[event.GetEventId()].begin();
                itr_receiver != this->event_to_receivers_map[event.GetEventId()].end();
                itr_receiver++)
            {
                threads.push_back(std::thread(this->event_receivers[*itr_receiver].GetCallback(), std::ref((Event &)event)));
            }

            for (int i = 0; i < threads.size(); i++)
            {
                threads.at(i).join();
            }

            ret = RET_OK;
        }
        else
        {
            ret = RET_NO_RECEIVERS_FOR_EVENT;
        }
    }

    return ret;
}