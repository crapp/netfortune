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

#include "fproto.hpp"

namespace
{
using json = nlohmann::json;
}

Fproto::Fproto() : message_length(0)
{
    this->logger = spdlog::get("multi_logger");
}

void Fproto::init_header(
    std::array<unsigned char, FPROTO_HEADER_SIZE> header_bytes)
{
    this->message_length = static_cast<unsigned short>(
        ((header_bytes.at(0) << 8) | header_bytes.at(1)));
    this->logger->debug("Message length " +
                        std::to_string(this->message_length) + " bytes");
    if (this->message_length > FPROTO_MESSAGE_MAX) {
        throw std::runtime_error("Header length of " +
                                 std::to_string(this->message_length) +
                                 " exceeds maximum ");
    }
}

void Fproto::read_message(const std::vector<unsigned char> &message_bytes)
{
    if (!message_bytes.empty()) {
        std::string msg_string(message_bytes.begin(), message_bytes.end());
        this->logger->info("Parsed string \"" + msg_string + "\"");
        try {
            this->request = json::parse(msg_string);
            this->logger->debug("Successfully parsed json");
        } catch (const std::exception &e) {
            this->logger->error("Invalid json encoding");
            this->logger->error(e.what());
        }
    } else {
        this->logger->info("Received message is empty");
    }
}

unsigned short Fproto::get_message_length() const
{
    return this->message_length;
}
