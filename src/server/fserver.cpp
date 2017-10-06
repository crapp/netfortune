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

#include "fserver.hpp"
#include "fsession.hpp"

#include "configuration.hpp"

#include <thread>

namespace bas = boost::asio;
namespace nc = netfortune_configuration;

FServer::FServer(boost::asio::io_service &io_service,
                 std::shared_ptr<cpptoml::table> cfg)
    : acceptor(io_service),
      socket(io_service),
      strand(io_service),
      cfg(std::move(cfg))
{
    this->logger = spdlog::get("multi_logger");
    this->logger->debug("Server object started");

    this->dbc = std::make_shared<DBCon>(this->cfg);

    // init the acceptor and bind it
    unsigned int port = this->cfg
                            ->get_qualified_as<unsigned int>(
                                nc::SERVER + std::string(".") + nc::SERVER_PORT)
                            .value_or(nc::SERVER_PORT_DEFAULT);
    // TODO: ipv6 not supported
    // FIXME: This will listen to all ip addresses.
    bas::ip::tcp::endpoint endpoint(bas::ip::tcp::v4(), port);
    this->acceptor.open(endpoint.protocol());
    this->acceptor.bind(endpoint);
    this->acceptor.listen();

    do_accept();
}

void FServer::do_accept()
{
    this->acceptor.async_accept(
        this->socket, this->strand.wrap([this](boost::system::error_code ec) {
            this->logger->debug("A client connected");
            if (!ec) {
                this->logger->debug("Starting new session");
                this->logger->info(
                    "New connection from " +
                    this->socket.remote_endpoint().address().to_string());
                std::make_shared<FSession>(std::move(this->socket))->start();
            }
            this->logger->debug_if(this->socket.is_open(), "Socket is open");

            do_accept();
        }));
}
