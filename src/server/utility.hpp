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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

#ifdef __GNUC__
#define ATTR_UNUSED __attribute__((unused))
#else
#define ATTR_UNUSED
#endif

/**
 * @brief Some utility functions for the project
 */
namespace netfortune_utility
{
/**
 * @brief Base case
 *
 * @tparam FirsT First parameter
 * @param first
 *
 * @return
 */
template <typename FirsT>
std::string toml_stringify(const FirsT &first)
{
    return first;
}

/**
 * @brief Function to chain to toml identifier strings with dots recursively
 *
 * @tparam T
 * @tparam FirsT
 * @param first First parameter
 * @param t Paramter pack
 *
 * @return Chained string
 */
template <typename... T, typename FirsT>
std::string toml_stringify(const FirsT &first, const T &... t)
{
    return first + std::string(".") + toml_stringify(t...);
}
}

#endif /* ifndef UTILITY_HPP */
