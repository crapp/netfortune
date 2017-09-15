// Netfortune server component
// Copyright © 2017 Christian Rapp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FSERVER_HPP
#define FSERVER_HPP

#include <boost/asio.hpp>

#include "spdlog/spdlog.h"

class FServer
{
public:
    FServer(boost::asio::io_service &io_service, short port);
    virtual ~FServer() = default;
    FServer(const FServer &) = delete;

private:
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    
    std::shared_ptr<spdlog::logger> console;

    void do_accept();
};

#endif /* FSERVER_HPP */
