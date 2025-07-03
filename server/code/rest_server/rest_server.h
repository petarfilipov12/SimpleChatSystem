#ifndef REST_SERVER_H
#define REST_SERVER_H

//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "external/httplib.h"

#include <string>
#include <functional>

#include "event_bus.h"

class RestServer
{
private:
    httplib::Server svr;
    std::string host;
    unsigned int port;

    EventBus& event_bus;


    void Post(const std::string& url_path, const std::function<void(const httplib::Request&, httplib::Response&)> handler_func);

    void InitServices();

    void Handler_KickUser(const httplib::Request &req, httplib::Response &res);
    void Handler_GetUsers(const httplib::Request &req, httplib::Response &res);

public:
    RestServer(EventBus& event_bus);

    RestServer(const std::string& host, const unsigned int port, EventBus& event_bus);

    ~RestServer();

    void run();
};

#endif