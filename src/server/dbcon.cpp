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

DBCon::~DBCon() { this->logger->debug("Closing sqlite dbhandle"); }

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
    sqlite::OpenFlags mutexflag = sqlite::OpenFlags::FULLMUTEX;
    if (sqlite3_threadsafe() == 0) {
        mutexflag = sqlite::OpenFlags::NOMUTEX;
    }

    sqlite::sqlite_config sqlconf;

    sqlconf.flags =
        sqlite::OpenFlags::READWRITE | sqlite::OpenFlags::CREATE | mutexflag;

    try {
        // open database in serial / mutex mode if possible
        this->db = std::make_unique<sqlite::database>(
            p.string() + boost::filesystem::path::separator + "netfortune.db",
            sqlconf);
        *this->db << "PRAGMA foreign_keys = ON";
    } catch (const sqlite::sqlite_exception &ex) {
        // throw error code and message
        throw std::runtime_error(std::string("Could not open DB. Error: ") +
                                 ex.what() + "\n" +
                                 std::to_string(ex.get_code()));
    }
}

void DBCon::init_database()
{
    this->logger->info("Initializing database");

    try {
        // clang-format off
        auto check_gen_exists(nu::stringify(
                    "SELECT * "
                    "  FROM sqlite_master"
                    " WHERE type='table'"
                    "   AND name='", dc::TABLE_GENERAL, "';"));
        // clang-format on
        bool netfortune_db_exists = false;
        bool needs_update = false;
        *this->db << check_gen_exists >> [&]() { netfortune_db_exists = true; };
        if (netfortune_db_exists) {
            // clang-format off
            auto check_db_version(
                    nu::stringify(
                        "SELECT ", dc::COL_GENERAL,
                        "  FROM ", dc::TABLE_GENERAL, ";"));
            // clang-format on
            *this->db << check_db_version >> [&](int version) {
                if (version != NETFORTUNE_DATABASE_VERSION)
                    needs_update = true;
            };
        }
        if (needs_update) {
            // check force switch as updates are destructive
            // automatically backup old database?
        } else if (netfortune_db_exists) {
            return;
        }

        // create general table with db version info
        // clang-format off
        auto crtgen(nu::stringify(
                    "CREATE TABLE ", dc::TABLE_GENERAL, "(",
                    dc::COL_GENERAL, " INTEGER NOT NULL);"
                    ));
        *this->db << crtgen;
        auto insgen(nu::stringify(
                    "INSERT into ", dc::TABLE_GENERAL, "(",
                    dc::COL_GENERAL, ")", "VALUES (?)"));
        *this->db << insgen << NETFORTUNE_DATABASE_VERSION;
        // clang-format on

        // create category table
        // clang-format off
        auto gencat(nu::stringify(
            "CREATE TABLE ", dc::TABLE_CATEGORY, "(",
            dc::COL_CATEGORY_ID,       " INTEGER PRIMARY KEY autoincrement NOT NULL, ",
            dc::COL_CATEGORY_TEXT,     " TEXT NOT NULL, ",
            dc::COL_CATEGORY_DATETIME, " TEXT NOT NULL);"));
        // clang-format on
        *this->db << gencat;

        // create fortunes table
        // clang-format off
        auto genfor(nu::stringify(
            "CREATE TABLE ", dc::TABLE_FORTUNE, "(",
            dc::COL_FORTUNE_ID,       " INTEGER PRIMARY KEY autoincrement NOT NULL, ",
            dc::COL_FORTUNE_TEXT,     " TEXT NOT NULL, ",
            dc::COL_FORTUNE_CATID,    " INTEGER NOT NULL, ",
            dc::COL_FORTUNE_DATETIME, " TEXT NOT NULL DEFAULT (datetime('now', 'localtime')), ",
            "FOREIGN KEY (", dc::COL_FORTUNE_CATID, ") REFERENCES ", dc::TABLE_CATEGORY, " (", dc::COL_CATEGORY_ID, ") ON DELETE CASCADE);"));
        // clang-format on
        *this->db << genfor;

        // create statistics table and an update trigger
        // clang-format off
        auto genstat(nu::stringify(
            "CREATE TABLE ", dc::TABLE_STAT, "(",
            dc::COL_STAT_ID,        " INTEGER PRIMARY KEY autoincrement NOT NULL, ",
            dc::COL_STAT_FORTUNEID, " INTEGER NOT NULL, ",
            dc::COL_STAT_CATID,     " INTEGER NOT NULL, ",
            dc::COL_STAT_NUM,       " INTEGER NOT NULL DEFAULT 1, ",
            dc::COL_STAT_DTUPDATE,  " TEXT NOT NULL DEFAULT (datetime('now', 'localtime')), ",
            dc::COL_STAT_DTCREATE,  " TEXT NOT NULL DEFAULT (datetime('now', 'localtime')), ",
            "FOREIGN KEY (", dc::COL_STAT_CATID, ") REFERENCES ", dc::TABLE_CATEGORY, " (", dc::COL_CATEGORY_ID, ") ON DELETE CASCADE, ",
            "FOREIGN KEY (", dc::COL_STAT_FORTUNEID, ") REFERENCES ", dc::TABLE_FORTUNE, " (", dc::COL_FORTUNE_ID, ") ON DELETE CASCADE);"));
        auto trigstat(nu::stringify(
            "CREATE TRIGGER update_stat_dtup AFTER UPDATE OF ", dc::COL_STAT_NUM, " ON ", dc::TABLE_STAT,
            " BEGIN ",
            "UPDATE ", dc::TABLE_STAT, " SET ", dc::COL_STAT_DTUPDATE, " = (datetime('now', 'localtime')) where ", dc::COL_STAT_ID, " = OLD.", dc::COL_STAT_ID, "; "
            " END;"));
        // clang-format on
        *this->db << genstat;
        *this->db << trigstat;
    } catch (const sqlite::sqlite_exception &ex) {
        this->throw_runtime(
            nu::stringify(ex.what(), "\n", ex.get_code(), "\n", ex.get_sql()));
    }
}
