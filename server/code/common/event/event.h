#ifndef EVENT_H
#define EVENT_H

namespace event
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
    private:
        eventId_t id;
        const void *data_in;
        void *data_out;

    public:
        Event(const eventId_t id, const void *data_in, void *data_out);
        ~Event();

        eventId_t GetEventId() const;

        const void *GetDataIn() const;

        void *GetDataOut() const;
    };
} // namespace event

#endif