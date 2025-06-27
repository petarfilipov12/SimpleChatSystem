#include <iostream>

#include "event_bus.h"
#include "event.h"

int main()
{
    EventBus event_bus;
    Event event(EVENT_ID_TEMP, nullptr, nullptr);

    std::cout << event_bus.Send(Event(EVENT_ID_INVALID, nullptr, nullptr)) << std::endl;
    std::cout << event_bus.Send(Event(EVENT_ID_TEMP, nullptr, nullptr)) << std::endl;

    return 0;
}