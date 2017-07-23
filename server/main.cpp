//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio.hpp>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>

#include "json.hpp"

using boost::asio::ip::tcp;

int main()
{
    auto make_daytime_string = []() -> std::string {
        std::time_t now = std::time(0);
        return ctime(&now);
    };
    try {
        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 13));

        for (;;) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            std::string message = make_daytime_string();
            message.erase(std::remove(message.begin(), message.end(), '\n'),
                          message.end());
            nlohmann::json j;
            j["daytime"] = message;

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(j.dump(2)),
                               ignored_error);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
