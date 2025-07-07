#include "event.h"
#include <cstring>
#include <cstdlib>

using namespace event_bus;

Event::Event()
{
    this->id = EVENT_ID_INVALID;
    this->data_in = nullptr;
    this->data_out = nullptr;
}

Event::Event(const eventId_t id, const void *data_in, const void *data_out)
{
    this->id = id;
    this->data_in = ((void *)data_in);
    this->data_out = ((void *)data_out);
}

eventId_t Event::GetEventId() const
{
    return this->id;
}

const void *Event::GetDataIn() const
{
    return ((const void *)this->data_in);
}

void *Event::GetDataOut() const
{
    return this->data_out;
}

EventAsync::EventAsync() : Event::Event()
{
}

EventAsync::EventAsync(const EventAsync &async_e)
{
    this->id = async_e.id;
    this->data_in = async_e.data_in;
    this->data_out = async_e.data_out;
}

EventAsync::EventAsync(const eventId_t id, const void *data_in, const void *data_out, const size_t event_data_in_size, const size_t event_data_out_size)
{
    void *data_in_copy = nullptr;

    if ((data_in != nullptr) && (event_data_in_size > 0))
    {
        data_in_copy = malloc(event_data_in_size);
        if (data_in_copy != NULL)
        {
            memcpy(data_in_copy, data_in, event_data_in_size);
        }
    }

    this->id = id;
    this->data_in = data_in_copy;
    this->data_out = ((void *)data_out);
}

void EventAsync::FreeData()
{
    if (this->data_in != nullptr)
    {
        free(this->data_in);
    }

    if (this->data_out != nullptr)
    {
        free(this->data_out);
    }
}

EventAsync &EventAsync::operator=(const EventAsync &async_e)
{
    this->id = async_e.id;
    this->data_in = async_e.data_in;
    this->data_out = async_e.data_out;

    return *this;
}
