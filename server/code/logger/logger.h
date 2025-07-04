#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <set>
#include <mutex>

#include "event.h"
#include "event_bus.h"

namespace logger
{
    class Logger
    {
    private:
        const std::string log_dir;
        std::set<std::string> backlog_files;
        std::string current_log_file;

        event_bus::EventReceiver event_receiver;

        std::mutex mtx;

        void SetCurrentLogName();

        void NewMessage(event_bus::Event &event);

        void EventHandler(event_bus::Event &event);

        void InitCycle();

        void Cyclic();

    public:
        Logger(const std::string &log_dir, const event_bus::eventReceiverId_t receiver_id, event_bus::EventBus &event_bus);

        void run();
    };
} // namespace logger

#endif