#include <iostream>

#include <chrono>

#include "event_bus.h"
#include "client_mgr.h"
#include "websocket_server.h"

int main()
{
    EventBus event_bus;
    eventReceiverId_t receiver_id = 0;
    std::vector<std::thread> threads;

    ClientMgr client_mgr(event_bus, receiver_id);
    WebsocketBroadcastServer ws_server(event_bus, 9002, ++receiver_id);

    client_mgr.run();
    ws_server.run();

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

