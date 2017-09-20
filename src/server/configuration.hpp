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

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <vector>

// configuration identifiers and default values

namespace netfortune_configuration
{
// global
const char* const CHANGED_CONFIG = "changed_config";
const bool CHANGED_CONFIG_DEFAULT = false;

// server
const char* const SERVER = "server";
const char* const SERVER_PORT = "port";
const unsigned int SERVER_PORT_DEFAULT = 13;
const char* const SERVER_LISTEN = "listen_to";
const char* const SERVER_LISTEN_DEFAULT = "127.0.0.1";

// fortunes

const char* const FORTUNES = "fortunes";
const char* const FORTUNES_BASE_FOLDER = "base_folder";
const char* const FORTUNES_BASE_FOlDER_DEFAULT = "/usr/share/fortune";
const char* const FORTUNES_BLACKLIST = "blacklist";
const std::vector<std::string> FORTUNES_BLACKLIST_DEFAULT = {};

// database

const char* const DATABASE = "database";
const char* const DATABASE_LOC = "location";
const char* const DATABASE_LOC_DEFAULT = "/var/lib/netfortune";
const char* const DATABASE_STATS = "create_stats";
const bool DATABASE_STATS_DEFAULT = true;

// logging

const char* const LOGGING = "logging";
const char* const LOGGING_FILE = "logfile";
const char* const LOGGING_FILE_DEFAULT = "/var/log/netfortune";
const char* const LOGFILE_MAX = "logfile_max";
const unsigned int LOGFILE_MAX_DEFAULT = 100;
const char* const LOGGING_STDOUT = "log_to_stdout";
const bool LOGGING_STDOUT_DEFAULT = true;
const char* const LOGGING_SYSLOG = "log_to_syslog";
const bool LOGGING_SYSLOG_DEFAULT = false;
const char* const LOGGING_MINLVL = "minimum_loglevel";
const char* const LOGGING_MINLVL_DEFAULT = "warning";
}

#endif /* ifndef CONFIGURATION_HPP */
