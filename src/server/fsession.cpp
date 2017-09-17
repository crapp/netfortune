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

#include "fsession.hpp"

namespace bas = boost::asio;

FSession::FSession(bas::ip::tcp::socket socket)
    : socket(std::move(socket)), nfprot(nullptr)
{
    this->logger = spdlog::get("multi_logger");
    this->logger->debug("FSession created");
}

void FSession::start()
{
    this->logger->debug("Session started");
    this->do_read();
}

void FSession::do_read()
{
    // https://stackoverflow.com/a/22291720
    // https://stackoverflow.com/a/25687309/1127601
    this->data_buf.clear();
    this->data_buf.resize(FPROTO_HEADER_SIZE);
    auto self = shared_from_this();
    this->logger->debug("FSession::do_read");
    // TODO: Simplify this code. Don't like these nested calls
    bas::async_read(
        this->socket, bas::buffer(this->data_buf, this->data_buf.size()),
        [this, self](boost::system::error_code ec, size_t bytes) {
            if (!ec) {
                this->nfprot = std::make_unique<Fproto>();
                try {
                    this->nfprot->init_header(
                        {{this->data_buf.at(0), this->data_buf.at(1)}});
                    this->data_buf.clear();
                    this->data_buf.resize(this->nfprot->get_message_length());
                    bas::async_read(
                        this->socket,
                        bas::buffer(this->data_buf, this->data_buf.size()),
                        [this, self](boost::system::error_code ec,
                                     size_t bytes) {
                            if (!ec) {
                                try {
                                    this->nfprot->read_message(this->data_buf);
                                } catch (const std::runtime_error &err) {
                                    this->logger->error(err.what());
                                }
                            }
                        });
                } catch (const std::out_of_range &ex) {
                    this->logger->error("CAN NOT PARSE HEADER BYTES");
                    return;
                } catch (const std::runtime_error &err) {
                    this->logger->error(err.what());
                    return;
                }
            } else {
                this->logger->debug("No data can be read in this session");
            }
        });
}

void FSession::do_write() {}
