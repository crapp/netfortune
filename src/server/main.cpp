#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

#include <boost/asio.hpp>

#include "spdlog/spdlog.h"

#include "config_netfortune.hpp"
#include "fserver.hpp"

int main()
{
    // asynchronous logging
    //spdlog::set_async_mode(512, spdlog::async_overflow_policy::discard_log_msg);
    auto console = spdlog::stdout_color_mt("console_logger");
    std::stringstream ss;
    ss << NETFORTUNE_VERSION_MAJOR << "." << NETFORTUNE_VERSION_MINOR;
#ifdef netfortune_VERSION_PATCH
    ss << "." << NETFORTUNE_VERSION_PATCH;
#endif

    console->info("Netfortune Server Version " + ss.str());
    boost::asio::io_service io_service;

    FServer s(io_service, 13);

    io_service.run();

    spdlog::drop_all();

    //boost::asio::io_service io;

    // try {
    // boost::asio::io_service io_service;

    // tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

    // for (;;) {
    // tcp::socket socket(io_service);
    //acceptor.accept(socket);

    // std::string message = make_daytime_string();
    //message.erase(std::remove(message.begin(), message.end(), '\n'),
    //message.end());
    // nlohmann::json j;
    // j["daytime"] = message;

    // boost::system::error_code ignored_error;
    //boost::asio::write(socket, boost::asio::buffer(j.dump(2)),
    // ignored_error);
    //}
    //} catch (std::exception& e) {
    //std::cerr << e.what() << std::endl;
    //}

    return 0;
}
