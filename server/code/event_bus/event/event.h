#ifndef EVENT_H
#define EVENT_H

#include <stddef.h>

#include <cstring>
#include <cstdlib>

#include "user.h"

namespace event_bus
{
    enum eEventId_t
    {
        EVENT_ID_TEMP,

        EVENT_ID_CONNECTION_OPENED,
        EVENT_ID_NEW_MESSAGE,
        EVENT_ID_CONNECTION_CLOSED,
        EVENT_ID_CONNECTION_FAILED,

        EVENT_ID_GET_USERS,
        EVENT_ID_DISCONNECT_USER,

        EVENT_ID_INVALID
    };

    typedef enum eEventId_t eventId_t;

    class Event
    {
    protected:
        eventId_t id;
        void *data_in;
        void *data_out;

    public:
        Event();

        Event(const eventId_t id, const void *data_in, const void *data_out);

        eventId_t GetEventId() const;

        const void *GetDataIn() const;

        void *GetDataOut() const;
    };

    class EventAsync: public Event
    {
    public:
        EventAsync();

        EventAsync(const EventAsync &async_e);

        EventAsync(const eventId_t id, const void *data_in, const void *data_out, const size_t event_data_in_size, const size_t event_data_out_size);
    
        void FreeData();

        EventAsync& operator=(const EventAsync &async_e);
    };
} // namespace event

#endif