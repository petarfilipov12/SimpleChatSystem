#include "event.h"

Event::Event(const eventId_t id, const void *data_in, void *data_out)
{
    this->id = id;
    this->data_in = data_in;
    this->data_out = data_out;
}
Event::~Event() {}

eventId_t Event::GetEventId() const
{
    return this->id;
}

const void* Event::GetDataIn() const
{
    return this->data_in;
}

void* Event::GetDataOut() const
{
    return this->data_out;
}
