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
    // TODO: Add multiple sinks to our logger
    // std::vector<spdlog::sink_ptr> sinks;
    //sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
    //sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logfile", "txt", 23, 59));
    // auto combined_logger = std::make_shared<spdlog::logger>("name",
    // begin(sinks), end(sinks));
    ////register it if you need to access it globally
    // spdlog::register_logger(combined_logger);
    // asynchronous logging
    spdlog::set_async_mode(512, spdlog::async_overflow_policy::discard_log_msg);
    auto logger = spdlog::stdout_color_mt("multi_logger");
    logger->set_level(spdlog::level::level_enum::debug);
    std::stringstream ss;
    ss << NETFORTUNE_VERSION_MAJOR << "." << NETFORTUNE_VERSION_MINOR;
#ifdef netfortune_VERSION_PATCH
    ss << "." << NETFORTUNE_VERSION_PATCH;
#endif

    logger->info("Netfortune Server Version " + ss.str());
    boost::asio::io_service io_service;

    boost::asio::signal_set signal_set(io_service, SIGINT, SIGTERM);
    signal_set.async_wait([&io_service, &logger](
        const boost::system::error_code &error, int signal_number) {
        std::unordered_map<int, std::string> signal_name = {
            {SIGINT, "SIGINT"}, {SIGTERM, "SIGTERM"}};
        logger->debug("Got signal " + signal_name.at(signal_number) +
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
