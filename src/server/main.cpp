#include <csignal>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>

#include "spdlog/spdlog.h"

#include "config_netfortune.hpp"
#include "fserver.hpp"

int main()
{
    // asynchronous logging
    spdlog::set_async_mode(512, spdlog::async_overflow_policy::discard_log_msg);
    auto console = spdlog::stdout_color_mt("console_logger");
    console->set_level(spdlog::level::level_enum::debug);
    std::stringstream ss;
    ss << NETFORTUNE_VERSION_MAJOR << "." << NETFORTUNE_VERSION_MINOR;
#ifdef netfortune_VERSION_PATCH
    ss << "." << NETFORTUNE_VERSION_PATCH;
#endif

    console->info("Netfortune Server Version " + ss.str());
    boost::asio::io_service io_service;

    boost::asio::signal_set signal_set(io_service, SIGINT, SIGTERM);
    signal_set.async_wait([&io_service, &console](
        const boost::system::error_code &error, int signal_number) {
        std::unordered_map<int, std::string> signal_name = {
            {SIGINT, "SIGINT"}, {SIGTERM, "SIGTERM"}};
        console->debug("Got signal " + signal_name.at(signal_number) +
                       "; stopping io_service.");
        io_service.stop();
    });

    FServer s(io_service, 13);

    io_service.run();

    spdlog::drop_all();

    // boost::asio::io_service io;

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
