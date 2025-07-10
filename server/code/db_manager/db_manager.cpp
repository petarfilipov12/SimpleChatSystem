#include "db_manager.h"

#include <map>
#include <memory>

#include "message.h"

using namespace db_manager;

void DbManager::NewMessage(event_bus::Event &event)
{
    std::shared_ptr<const common::Message> p_message = std::static_pointer_cast<const common::Message>(event.GetDataIn());

    std::map<std::string, std::string> column_val_map = {
        {"Username", p_message->GetUser().GetUsername()},
        {"Message", p_message->GetText()},
        {"Date", p_message->GetDate()}};

    this->db_wrapper.InsertRow(this->table_name, column_val_map);
}

void DbManager::EventHandler(event_bus::Event &event)
{
    switch (event.GetEventId())
    {
    case event_bus::EVENT_ID_NEW_MESSAGE:
        this->NewMessage(event);
        break;
    default:
        break;
    }
}

DbManager::DbManager(const std::string &table_name, const std::string &db_path, const event_bus::eventReceiverId_t receiver_id, event_bus::EventBus &event_bus) : table_name(table_name)
{
    this->db_wrapper = DbWrapper(db_path);

    std::map<std::string, std::string> column_type_map = {
        {"Username", "TEXT"},
        {"Message", "TEXT"},
        {"Date", "TEXT"}};

    this->db_wrapper.CreateTable(table_name, column_type_map);

    this->event_receiver = event_bus::EventReceiver(receiver_id, std::bind(&DbManager::EventHandler, this, std::placeholders::_1));

    event_bus.AddReceiver(this->event_receiver);
    event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_NEW_MESSAGE);
}