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
namespace dc = dbcon_constants;

DBCon::DBCon(std::shared_ptr<cpptoml::table> cfg) : cfg(std::move(cfg))
{
    this->logger = spdlog::get("multi_logger");
    this->init_connection();
    this->init_database();
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
    this->logger->info("Initializing connection");
    // check if path exists
    std::string path = this->cfg
                           ->get_qualified_as<std::string>(nu::toml_stringify(
                               nc::DATABASE, nc::DATABASE_LOC))
                           .value_or(nc::DATABASE_LOC_DEFAULT);
    boost::filesystem::path p(path);
    // this will throw an exception if it doesn't work
    boost::filesystem::create_directories(p);
    // check if the sqlite lib is able to run in serialized mode
    int mutex_mode = SQLITE_OPEN_FULLMUTEX;
    if (sqlite3_threadsafe() == 0) {
        mutex_mode = SQLITE_OPEN_NOMUTEX;
    }
    // open database in serial / mutex mode if possible
    int rc = sqlite3_open_v2(
        std::string(p.string() + boost::filesystem::path::separator +
                    "netfortune.db")
            .c_str(),
        &this->dbhandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | mutex_mode,
        nullptr);
    if (rc != SQLITE_OK) {
        // throw error code and message
        throw std::runtime_error(
            "Could not open DB. Error: " + std::string(sqlite3_errstr(rc)) +
            "\n" + std::string(sqlite3_errmsg(this->dbhandle)));
    }
}

void DBCon::init_database()
{
    this->logger->info("Initializing database");

    char *errMsg = nullptr; // make sure to free this one with sqlite3_free()
    auto err_handler = [this, errMsg](const auto &msg) {
        this->logger->error(errMsg);
        sqlite3_free(errMsg);
        this->throw_runtime("Error querrying database");
    };

    using gen_exists_arg = std::tuple<bool, dc::GENERAL_MAP>;
    gen_exists_arg gen_result;
    std::get<0>(gen_result) = false;
    std::string check_gen_exists("SELECT * "
                                 "  FROM table_master"
                                 " WHERE type='table'"
                                 "   AND name='" +
                                 std::string(dc::TABLE_GENERAL) + "';");

    int rc =
        sqlite3_exec(this->dbhandle, check_gen_exists.c_str(),
                     [](void *gen_result, int count, char **data ATTR_UNUSED,
                        char **columns ATTR_UNUSED) -> int {
                         if (count != 0) {
                             auto *local_gen_result =
                                 static_cast<gen_exists_arg *>(gen_result);
                             std::get<0>(*local_gen_result) = true;
                         }
                         return 0;
                     },
                     &gen_result, &errMsg);
    if (rc != 0) {
        err_handler(errMsg);
    }

    bool needs_update = false;

    if (std::get<0>(gen_result)) {
        std::string check_db_version("SELECT " + std::string(dc::COL_GENERAL) +
                                     "  FROM " + dc::TABLE_GENERAL + ";");
        rc = sqlite3_exec(this->dbhandle, check_db_version.c_str(),
                          [](void *gen_result, int count, char **data,
                             char **column_names) -> int {
                              auto *local_gen_result =
                                  static_cast<gen_exists_arg *>(gen_result);
                              for (int i = 0; i < count; i++) {
                                  std::get<1>(*local_gen_result)
                                      .insert(std::make_pair(
                                          column_names[i], std::stoi(data[i])));
                              }
                              return 0;
                          },
                          &gen_result, &errMsg);
        if (rc != 0) {
            err_handler(errMsg);
        }
        if (std::get<1>(gen_result).at(dc::COL_GENERAL) !=
            NETFORTUNE_DATABASE_VERSION) {
            needs_update = true;
        }
    }

    std::string sql_create_category("CREATE TABLE if not exists " +
                                    std::string(dc::TABLE_CATEGORY) +
                                    "("
                                    "id INTEGER PRIMARY KEY,"
                                    "category TEXT UNIQUE,"
                                    ")");
    // rc = sqlite3_exec(this->dbhandle, )
}
