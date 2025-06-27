#ifndef EVENT_H
#define EVENT_H

enum eEventId_t
{
    EVENT_ID_TEMP,

    EVENT_ID_INVALID
};

typedef enum eEventId_t eventId_t;

class Event{
    private:
        eventId_t id;
        const void* data_in;
        void* data_out;

    public:
        Event(const eventId_t id, const void* data_in, void* data_out)
        {
            this->id = id;
            this->data_in = data_in;
            this->data_out = data_out;
        }
        ~Event() {}

        eventId_t GetEventId() const
        {
            return this->id;
        }

        const void* GetDataIn() const
        {
            return this->data_in;
        }

        void* GetDataOut() const
        {
            return this->data_out;
        }
};

#endif