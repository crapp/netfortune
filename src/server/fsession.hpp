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

#ifndef FSESSION_HPP
#define FSESSION_HPP

#include <cassert>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "spdlog/spdlog.h"

#include "fproto.hpp"
#include "utility.hpp"

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
    explicit FSession(boost::asio::ip::tcp::socket socket);
    virtual ~FSession() = default;

    /**
     * @brief Start the reading on FSession::socket
     */
    void start();

private:
    std::vector<unsigned char>
        data_buf; /**< buffer containing the request and later the answer */
    const size_t max_buf_size = 2048;    /**< maximum data buffer size */
    boost::asio::ip::tcp::socket socket; /**< Socket this session uses */
    boost::asio::strand strand;          /**< synchronize asio calls */

    std::shared_ptr<spdlog::logger> logger;
    std::unique_ptr<Fproto> nfprot;

    void do_read();
    void do_write();
};

#endif /* FSESSION_HPP */
