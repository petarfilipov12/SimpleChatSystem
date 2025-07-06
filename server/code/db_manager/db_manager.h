#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <string>

#include "db_wrapper.h"

#include "event.h"
#include "event_bus.h"

namespace db_manager
{
    class DbManager
    {
    private:
        db_manager::DbWrapper db_wrapper;
        const std::string table_name;

        event_bus::EventReceiver event_receiver;

        void NewMessage(event_bus::Event &event);

        void EventHandler(event_bus::Event &event);

    public:
        DbManager(const std::string& table_name, const std::string& db_path, const event_bus::eventReceiverId_t receiver_id, event_bus::EventBus &event_bus);
    };
}//namespace db_manager

#endif