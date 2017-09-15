// Netfortune data exchange protocol
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

#ifndef FPROTO_HPP
#define FPROTO_HPP

#include <stdexcept>

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
    void init_header(std::array<unsigned char, 2> header_bytes);

private:
    unsigned short message_length;
};

#endif /* FPROTO_HPP */
