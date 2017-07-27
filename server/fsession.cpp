// netfortune session
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
//
#include "fsession.hpp"

#include <thread>

namespace bas = boost::asio;

FSession::FSession(bas::ip::tcp::socket socket) : socket(std::move(socket))
{
    std::cout << "FSession created" << std::endl;
}

FSession::~FSession() { std::cout << "FSession destroyed" << std::endl; }

void FSession::start()
{
    std::cout << "Session started" << std::endl;
    // this will block
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void FSession::do_read()
{
    // https://stackoverflow.com/a/22291720
    // https://stackoverflow.com/a/25687309/1127601
    this->data_buf.clear();
    this->data_buf.resize(2);
    auto self(shared_from_this());
    bas::async_read(
        this->socket, bas::buffer(this->data_buf, this->data_buf.size()),
        [this, self](boost::system::error_code ec, size_t bytes) {
            unsigned short int message_length = 0;
            if (!ec) {
                assert(bytes == 2);
                message_length =
                    ((this->data_buf.at(0) << 8) & this->data_buf.at(1));
                std::cout << message_length << std::endl;
            }
        });
}

void FSession::do_write() {}
