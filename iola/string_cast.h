/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_STRING_CAST_H
#define IOLA_STRING_CAST_H

// Iola NLE
// Copyright (c) 2010, Anders Dahnielson
//
// Contact: anders@dahnielson.com
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

//NOTE: This code was nicked from K-3D ...

// BOOST
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

// STD
#include <limits>
#include <sstream>
#include <string>

namespace iola
{

///////////////////////////////////////////////////////////////////////////
// string_cast

/// Converts any serializable type to a string
template<typename type>
const std::string string_cast(const type& RHS)
{
    std::ostringstream buffer;
    if(std::numeric_limits<type>::is_specialized)
        buffer.precision(std::numeric_limits<type>::digits10 + 1);

    buffer << RHS;
    return buffer.str();
}

/// Specialization of string_cast for type bool
template<>
inline const std::string string_cast<bool>(const bool& RHS)
{
    return RHS ? "true" : "false";
}

/// Specialization of string_cast for strings
template<>
inline const std::string string_cast<std::string>(const std::string& RHS)
{
    return RHS;
}

/// Specialization of string_cast() for boost::filesystem::path
template<>
inline const std::string string_cast<boost::filesystem::path>(const boost::filesystem::path& RHS)
{
    return RHS.native_file_string();
}

/// Specialization of string_cast() for boost::format
template<>
inline const std::string string_cast<boost::format>(const boost::format& RHS)
{
    return RHS.str();
}

} // namespace iola

#endif // !IOLA_STRING_CAST_H
