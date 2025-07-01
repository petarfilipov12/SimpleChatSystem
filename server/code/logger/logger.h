#ifndef LOGGER_H
#define LOGGER_H

#include "return_type.h"

#include <string>
#include <set>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

#include "file_handler.h"
#include "event.h"
#include "event_bus.h"
#include "message.h"

#define HOUR_S (60*60)

class Logger
{
private:
    const std::string log_dir;
    std::set<std::string> backlog_files;
    std::string current_log_file;

    EventReceiver event_receiver;

    void SetCurrentLogName()
    {
        time_t t = time(nullptr);
        struct tm* date = localtime(&t);

        this->current_log_file = (
            std::to_string(1900 + date->tm_year) + 
            "-" + std::to_string(date->tm_mon) + 
            "-" + std::to_string(date->tm_mday) + 
            "_" + std::to_string(date->tm_hour) + ".log"
        );

        this->backlog_files.insert(this->current_log_file);
    }

    void NewMessage(Event& event)
    {
        Message* p_message = (Message*)event.GetDataIn();
        file_handler::AppendOrWrite(this->log_dir + "/" + this->current_log_file, p_message->ToString() + '\n');
    }

    void EventHandler(Event& event)
    {
        switch(event.GetEventId())
        {
            case EVENT_ID_NEW_MESSAGE:
                this->NewMessage(event);
                break;
            default:
                break;
        }
    }

    void InitCycle()
    {
        time_t sleep_time = HOUR_S - (time(nullptr) % HOUR_S) + 1;
        this->SetCurrentLogName();
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    }

    void Cyclic()
    {
        while (true)
        {
            this->SetCurrentLogName();
            std::this_thread::sleep_for(std::chrono::seconds(HOUR_S));
        }
    }

public:
    Logger(const std::string& log_dir, const eventReceiverId_t receiver_id, EventBus& event_bus): log_dir(log_dir)
    {
        file_handler::GetFiles(this->log_dir, this->backlog_files);

        this->event_receiver = EventReceiver(receiver_id, std::bind(&Logger::EventHandler, this, std::placeholders::_1));

        event_bus.AddReceiver(this->event_receiver);
        event_bus.Subscribe(receiver_id, EVENT_ID_NEW_MESSAGE);
    }

    void run()
    {
        std::thread t([this]{
            this->InitCycle();
            this->Cyclic();
        });
        t.detach();
    }

};

#endif