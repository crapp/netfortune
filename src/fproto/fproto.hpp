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

#ifndef FPROTO_HPP
#define FPROTO_HPP

#include <stdexcept>

#include "spdlog/spdlog.h"

#include "config_netfortune.hpp"
#include "json.hpp"

class Fproto
{
public:
    Fproto();
    virtual ~Fproto() = default;
    Fproto(const Fproto&) = delete;
    Fproto(Fproto&&) = default;
    Fproto& operator=(const Fproto&) = delete;

    /**
     * @brief Parse header bytes to get message length
     *
     * @param header_bytes The first two bytes copied from the session buffer
     */
    void init_header(std::array<unsigned char, FPROTO_HEADER_SIZE> header_bytes);

    void read_message(const std::vector<unsigned char>& message_bytes);

    unsigned short get_message_length() const;

private:
    unsigned short message_length;

    nlohmann::json request;

    std::shared_ptr<spdlog::logger> logger;
};

#endif /* FPROTO_HPP */
