#include <iostream>

#include <chrono>

#include "event_bus.h"
#include "client_mgr.h"
#include "websocket_server.h"
#include "logger.h"

#define WEBSOCKET_PORT 9002
#define MESSEGES_LOGS_DRI "./logs"

int main()
{
    eventReceiverId_t receiver_id = 0;
    std::vector<std::thread> threads;

    EventBus event_bus;
    ClientMgr client_mgr(event_bus, receiver_id);
    Logger logger(MESSEGES_LOGS_DRI, ++receiver_id, event_bus);
    WebsocketBroadcastServer ws_server(event_bus, WEBSOCKET_PORT, ++receiver_id);

    client_mgr.run();
    logger.run();
    ws_server.run();

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

