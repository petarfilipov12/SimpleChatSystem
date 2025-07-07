#include <iostream>

#include <chrono>

#include "event_bus.h"
#include "client_mgr.h"
#include "websocket_server.h"
#include "rest_server.h"
#include "logger.h"
#include "db_manager.h"

#define MESSEGES_LOGS_DIR "./logs"

#define DB_TABLE_NAME "UserMessages"
#define DB_PATH "database.db"

#define WEBSOCKET_PORT 9002

#define REST_SERVER_HOST "0.0.0.0"
#define REST_SERVER_PORT 8080

int main()
{
    event_bus::eventReceiverId_t receiver_id = 0;

    event_bus::EventBus event_bus;
    //event_bus.run();
    
    client_mgr::ClientMgr client_mgr(event_bus, receiver_id);
    logger::Logger logger(MESSEGES_LOGS_DIR, ++receiver_id, event_bus);
    db_manager::DbManager db_manager(DB_TABLE_NAME, DB_PATH, ++receiver_id, event_bus);
    websocket_server::WebsocketServer ws_server(event_bus, WEBSOCKET_PORT, ++receiver_id);
    rest_server::RestServer rest_server(REST_SERVER_HOST, REST_SERVER_PORT, event_bus);

    client_mgr.run();
    logger.run();
    ws_server.run();
    rest_server.run();

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

