#include <iostream>

#include "event_bus.h"
#include "event.h"

void Handler(Event& event)
{
    if(event.GetEventId() == EVENT_ID_TEMP)
    {
        std::cout << "Handler" << std::endl;
        *((int*)event.GetDataOut()) = *((int*)event.GetDataIn()) + 5;
    }
}

int main()
{
    EventBus event_bus;

    Event event(EVENT_ID_TEMP, nullptr, nullptr);
    std::cout << event_bus.Send(event) << std::endl;

    event = Event(EVENT_ID_INVALID, nullptr, nullptr);
    std::cout << event_bus.Send(event) << std::endl;


    std::cout << "TUK" << std::endl;
    EventReceiver event_receiver(0, Handler);
    std::cout << event_bus.AddReceiver(event_receiver) << std::endl;
    std::cout << event_bus.Subscribe(0, EVENT_ID_TEMP) << std::endl;

    int data_in = 5;
    int data_out;
    event = Event(EVENT_ID_TEMP, &data_in, &data_out);
    std::cout << event_bus.Send(event) << std::endl;
    std::cout << data_in << std::endl;
    std::cout << data_out << std::endl;

    return 0;
}

