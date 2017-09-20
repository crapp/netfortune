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

#ifndef DBCON_HPP
#define DBCON_HPP

#include <memory>

#include <sqlite3.h>

#include "cpptoml/cpptoml.h"
#include "spdlog/spdlog.h"

#include "configuration.hpp"

/**
 * @brief DB related constants. Mostly table and column names
 */
namespace dbcon_constants
{
const char* const TABLE_FORTUNE = "fortune";
const char* const TABLE_FORTUNE_ID = "id";
const char* const TABLE_FORTUNE_CATEGORY = "category";
const char* const TABLE_FORTUNE_TEXT = "text";
const char* const TABLE_FORTUNE_DATETIME = "datetime";

const char* const TABLE_CATEGORY = "category";
const char* const TABLE_CATEGORY_ID = "id";
const char* const TABLE_CATEGORY_TEXT = "text";
const char* const TABLE_CATEGORY_DATETIME = "datetime";

const char* const TABLE_STAT = "statistic";
}

/**
 * @brief Connection to sqlite database
 */
class DBCon
{
public:
    DBCon();
    virtual ~DBCon() = default;
    DBCon(const DBCon&) = delete; /**< no copy constructor **/

private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<cpptoml::table> cfg;
};

#endif /* DBCON_HPP */