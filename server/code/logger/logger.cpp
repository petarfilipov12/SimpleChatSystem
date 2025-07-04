#include "logger.h"

#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>

#include "file_handler.h"
#include "message.h"

#define HOUR_S (60 * 60)

using namespace logger;

void Logger::SetCurrentLogName()
{
    time_t t = time(nullptr);
    struct tm *date = localtime(&t);

    std::lock_guard<std::mutex> guard_mtx(this->mtx);
    this->current_log_file = (std::to_string(1900 + date->tm_year) +
                              "-" + std::to_string(date->tm_mon) +
                              "-" + std::to_string(date->tm_mday) +
                              "_" + std::to_string(date->tm_hour) + ".log");

    this->backlog_files.insert(this->current_log_file);
}

void Logger::NewMessage(event_bus::Event &event)
{
    common::Message *p_message = (common::Message *)event.GetDataIn();

    std::lock_guard<std::mutex> guard_mtx(this->mtx);
    common::FileAppendOrWrite(this->log_dir + "/" + this->current_log_file, p_message->ToString() + '\n');
}

void Logger::EventHandler(event_bus::Event &event)
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

void Logger::InitCycle()
{
    time_t sleep_time = HOUR_S - (time(nullptr) % HOUR_S) + 1;
    this->SetCurrentLogName();
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
}

void Logger::Cyclic()
{
    while (true)
    {
        this->SetCurrentLogName();
        std::this_thread::sleep_for(std::chrono::seconds(HOUR_S));
    }
}

Logger::Logger(const std::string &log_dir, const event_bus::eventReceiverId_t receiver_id, event_bus::EventBus &event_bus) : log_dir(log_dir)
{
    common::FileGetFiles(this->log_dir, this->backlog_files);

    this->event_receiver = event_bus::EventReceiver(receiver_id, std::bind(&Logger::EventHandler, this, std::placeholders::_1));

    event_bus.AddReceiver(this->event_receiver);
    event_bus.Subscribe(receiver_id, event_bus::EVENT_ID_NEW_MESSAGE);
}

void Logger::run()
{
    std::thread t([this]{
            this->InitCycle();
            this->Cyclic(); });
    t.detach();
}
