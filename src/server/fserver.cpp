// Netfortune server component
// Copyright © 2017 Christian Rapp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "fserver.hpp"
#include "fsession.hpp"

namespace bas = boost::asio;

FServer::FServer(boost::asio::io_service &io_service, short port)
    : acceptor(io_service, bas::ip::tcp::endpoint(bas::ip::tcp::v4(), port)),
      socket(io_service)
{
    this->logger = spdlog::get("multi_logger");
    this->logger->debug("Server object started");
    do_accept();
}

void FServer::do_accept()
{
    this->acceptor.async_accept(
        this->socket, [this](boost::system::error_code ec) {
            this->logger->debug("A client connected");
            if (!ec) {
                this->logger->debug("EC OK starting my session");
                this->logger->info(
                    "New connection from " +
                    this->socket.remote_endpoint().address().to_string());
                std::make_shared<FSession>(std::move(this->socket))->start();
            }
            this->logger->debug_if(this->socket.is_open(), "Socket is open");

            do_accept();
        });
    this->logger->debug("After accept");
}
