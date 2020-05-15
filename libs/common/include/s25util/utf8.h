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

#ifndef ucString_h__
#define ucString_h__

#include <string>

namespace s25util {

/// Convert an UTF-8 string to unicode string
std::u32string utf8to32(const std::string& other);

/// Convert an unicode string to UTF-8 string
std::string utf32to8(const std::u32string& other);

/// Return true if text is valid UTF-8
bool isValidUTF8(const std::string& text);
std::string::const_iterator findInvalidUTF8(const std::string& text);

/// Convert ANSI (aka Windows-1252) encoded strings to UTF-8
std::string ansiToUTF8(const std::string& other);

} // namespace s25util

#endif // ucString_h__