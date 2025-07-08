#include "event.h"
#include <cstring>
#include <cstdlib>

using namespace event_bus;

Event::Event() : data_in(nullptr)
{
    this->id = EVENT_ID_INVALID;
    this->data_out = nullptr;
}

Event::Event(const eventId_t id, const std::any &data_in, const std::any& data_out)
{
    this->id = id;
    this->data_in = data_in;
    this->data_out = data_out;
}

const eventId_t Event::GetEventId() const
{
    return this->id;
}

const std::any &Event::GetDataIn() const
{
    return this->data_in;
}

std::any Event::GetDataOut() const
{
    return this->data_out;
}

Event &Event::operator=(const Event &e)
{
    this->id = e.id;
    this->data_in = e.data_in;
    this->data_out = e.data_out;

    return *this;
}
