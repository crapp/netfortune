// netfortune brings fortunes for everyone, everywhere
// Copyright © 2017 Christian Rapp
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// 3. Neither the name of the organization nor the
// names of its contributors may be used to endorse or promote products
// derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY  ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL  BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <csignal>
#include <ctime>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include "cpptoml/cpptoml.h"
#include "spdlog/spdlog.h"

#include "config_netfortune.hpp"
#include "configuration.hpp"
#include "fserver.hpp"
#include "utility.hpp"

int main()
{
    std::shared_ptr<cpptoml::table> cfg = nullptr;
    std::string cfg_file = "netfortune-server.toml";
    if (!boost::filesystem::exists(cfg_file)) {
        if (!boost::filesystem::exists("/etc/" + cfg_file)) {
            std::cerr << "Can not find config file netfortune-server.toml"
                      << std::endl;
            return 1;
        }
        cfg_file = "/etc/" + cfg_file;
    }
    try {
        cfg = cpptoml::parse_file("netfortune-server.toml");
    } catch (const cpptoml::parse_exception &ex) {
        std::cerr << "Could not parse config file" << std::endl;
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    namespace nc = netfortune_configuration;
    // check if config file was changed
    bool config_changed = cfg->get_as<bool>(nc::CHANGED_CONFIG)
                              .value_or(nc::CHANGED_CONFIG_DEFAULT);
    if (!config_changed) {
        std::cerr
            << "You have to change the configuration file for netfortune server"
            << std::endl;
        return 1;
    }
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

    logger->debug(netfortune_utility::toml_stringify(
        nc::CHANGED_CONFIG, nc::DATABASE, nc::FORTUNES));

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

    try {
        FServer s(io_service, std::move(cfg));
        io_service.run();
    } catch (const std::exception &ex) {
        logger->error(ex.what());
    }

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
