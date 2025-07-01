#ifndef CLIENT_MGR_H
#define CLIENT_MGR_H

#include "return_type.h"

#include <map>
#include <ctime>
#include <chrono>

#include "event.h"
#include "event_bus.h"
#include "user.h"
#include "message.h"

class ClientMgr
{
private:
    EventBus& event_bus;
    EventReceiver event_receiver;
    const time_t timeout = 10*60; //10min

    std::map<User, time_t> user_timeouts;

    void ConnectionOpened(Event& event)
    {
        User* p_user = (User*)event.GetDataIn();
        this->user_timeouts[*p_user] = time(nullptr);
    }

    void ConnectionClosed(Event& event)
    {
        User* p_user = (User*)event.GetDataIn();
        this->user_timeouts.erase(*p_user);
    }

    void NewMessage(Event& event)
    {
        Message* p_message = (Message*)event.GetDataIn();
        this->user_timeouts[p_message->GetUser()] = time(nullptr);
    }

    void EventHandler(Event& event)
    {
        switch(event.GetEventId())
        {
            case EVENT_ID_CONNECTION_OPENED:
                this->ConnectionOpened(event);
                break;
            case EVENT_ID_CONNECTION_CLOSED:
                this->ConnectionClosed(event);
                break;
            case EVENT_ID_NEW_MESSAGE:
                this->NewMessage(event);
                break;
            default:
                break;
        }
    }

    void Cyclic()
    {
        while(true)
        {
            for(auto it = this->user_timeouts.begin(); it != this->user_timeouts.end(); it++)
            {
                if(time(nullptr) >= (it->second + this->timeout))
                {
                    this->event_bus.Send(Event(EVENT_ID_DISCONNECT_USER, &(it->first), nullptr));
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

public:
    ClientMgr(EventBus& event_bus, const eventReceiverId_t receiver_id): event_bus(event_bus)
    {
        this->event_receiver = EventReceiver(receiver_id, std::bind(&ClientMgr::EventHandler, this, std::placeholders::_1));

        this->event_bus.AddReceiver(this->event_receiver);
        this->event_bus.Subscribe(receiver_id, EVENT_ID_CONNECTION_OPENED);
        this->event_bus.Subscribe(receiver_id, EVENT_ID_CONNECTION_CLOSED);
        this->event_bus.Subscribe(receiver_id, EVENT_ID_NEW_MESSAGE);
    }

    void run()
    {
        std::thread t([this]{this->Cyclic();});
        t.detach();
    }
};

#endif