// Copyright (c) 2005 - 2017 Settlers Freaks (sf-team at siedler25.org)
//
// This file is part of Return To The Roots.
//
// Return To The Roots is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Return To The Roots is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Return To The Roots. If not, see <http://www.gnu.org/licenses/>.
#ifndef STR_FUNCS_H_INCLUDED
#define STR_FUNCS_H_INCLUDED

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>

// Provides safe C-String functions by preventing buffer overflows
// Better use std::string and boost::format!

namespace detail {

/// Copies a C-String into a buffer (with terminating zero)
/// Throws an exception on buffer overflow (strlen(pSrc) >= maxChars)
void strcpyExt(char* pOut, const char* pSrc, size_t maxChars);
} // namespace detail

/// Copies a C-String into a buffer (with terminating zero)
/// Throws an exception on buffer overflow (strlen(pSrc) >= T_size)
template<size_t T_size>
void strcpy_check(std::array<char, T_size>& output, const size_t offset, const char* pSrc)
{
    assert(offset <= T_size);
    detail::strcpyExt(output.data() + offset, pSrc, T_size - offset);
}

/// Create a random string of length len using the given categories
std::string createRandString(size_t len, bool useLowercase = true, bool useUppercase = true, bool useNumbers = true,
                             bool useSpecialChars = true);
/// Create a random string with chars from the given charset
std::string createRandString(size_t len, const std::string& charset);
/// Create a random string with chars from the given charset and seed
std::string createRandString(size_t len, const std::string& charset, uint32_t seed);

#endif // STR_FUNCS_H_INCLUDED