// Netfortune brings fortunes to everyone everywhere
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

// THIS SOFTWARE IS PROVIDED BY Christian Rapp ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Christian Rapp  BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "dbcon.hpp"

#include <boost/filesystem.hpp>

#include "configuration.hpp"
#include "utility.hpp"

namespace nc = netfortune_configuration;
namespace nu = netfortune_utility;

DBCon::DBCon(std::shared_ptr<cpptoml::table> cfg) : cfg(std::move(cfg))
{
    this->logger = spdlog::get("multi_logger");
    this->init_connection();
}

DBCon::~DBCon()
{
    this->logger->debug("Closing sqlite dbhandle");
    if (this->dbhandle != nullptr) {
        sqlite3_close(dbhandle);
    }
}

void DBCon::init_connection()
{
    // check if path exists
    std::string path = this->cfg
                           ->get_qualified_as<std::string>(nu::toml_stringify(
                               nc::DATABASE, nc::DATABASE_LOC))
                           .value_or(nc::DATABASE_LOC_DEFAULT);
    boost::filesystem::path p(path);
    // this will throw an exception if it doesn't work
    boost::filesystem::create_directories(p);
    // open databse in serial / mutex mode
    int rc = sqlite3_open_v2(
        std::string(p.string() + boost::filesystem::path::separator +
                    "netfortune.db")
            .c_str(),
        &this->dbhandle,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
        nullptr);
    if (rc != SQLITE_OK) {
        // throw error code and message
        throw std::runtime_error(
            "Could not open DB. Error: " + std::string(sqlite3_errstr(rc)) +
            "\n" + std::string(sqlite3_errmsg(this->dbhandle)));
    }
}
