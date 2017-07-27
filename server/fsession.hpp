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

#ifndef FSESSION_HPP
#define FSESSION_HPP

#include <cassert>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "fproto.hpp"

/**
 * @brief FSession class represents one TCP Connection from server to client
 * @detail
 * A session handels reading and writing from/to the client.
 * 1. Reading the first 6 Bytes to get the actual message length. Then read
 *    the rest.
 * 2. Parse the message with a json encoder and see if this is valid json
 * 3. Get the information we need from the client and prepare the answer
 *    accordingly
 *
 */
class FSession : public std::enable_shared_from_this<FSession>
{
public:
    FSession(boost::asio::ip::tcp::socket socket);
    virtual ~FSession();

    /**
     * @brief Start the reading on FSession::socket
     */
    void start();

private:
    std::vector<char>
        data_buf; /**< buffer containing the request and later the answer */
    const size_t max_buf_size = 2048;    /**< maximum data buffer size */
    boost::asio::ip::tcp::socket socket; /**< Socket this session uses */

    void do_read();
    void do_write();
};

#endif /* FSESSION_HPP */
