#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <thread>
#include <string>

typedef websocketpp::client<websocketpp::config::asio_client> client;

client c;
websocketpp::connection_hdl g_hdl;

void on_message(websocketpp::connection_hdl hdl, client::message_ptr msg) {
	std::cout << msg->get_payload() << std::endl;
}

void on_open(websocketpp::connection_hdl hdl) {
	g_hdl = hdl;
}

int main() {
    std::string uri = "ws://localhost:9002";

	try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);
        c.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(&on_message);
        c.set_open_handler(&on_open);

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        std::string username;
        std::cout << "USERNAME: ";
        std::cin >> username;
        con->append_header("Username", username);

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.

        std::thread t([]{c.run();});
        t.detach();

        std::string msg;
        while(true)
        {
            std::cout << "MSG: ";
            std::cin >> msg;

            if(msg == "QUIT")
            {
                c.close(g_hdl, websocketpp::close::status::normal, "GEI");
                break;
            }
            c.send(g_hdl, msg, websocketpp::frame::opcode::text);
        }
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}