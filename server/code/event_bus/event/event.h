#ifndef EVENT_H
#define EVENT_H

#include <stddef.h>

#include <cstring>
#include <cstdlib>
#include <memory>

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
        std::shared_ptr<const void> data_in;
        std::shared_ptr<void> data_out;

    public:
        Event();

        Event(const eventId_t id, const std::shared_ptr<const void>& data_in, const std::shared_ptr<void>& data_out);

        const eventId_t GetEventId() const;

        const std::shared_ptr<const void>& GetDataIn() const;

        const std::shared_ptr<void>& GetDataOut() const;

        Event& operator=(const Event &e);
    };
} // namespace event

#endif