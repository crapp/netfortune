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

class FSession : public std::enable_shared_from_this<FSession>
{
public:
    FSession(boost::asio::ip::tcp::socket socket);
    virtual ~FSession();

    void start();

private:
    boost::asio::ip::tcp::socket socket;
    void do_read();
};

#endif /* FSESSION_HPP */
