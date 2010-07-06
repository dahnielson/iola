/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_XML_PARSER_H
#define IOLA_XML_PARSER_H

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

// STD
#include <iostream>

// IOLA
#include "ielement.h"
#include "ifactory.h"

namespace iola
{
namespace xml
{

////////////////////////////////////////////////////////////////////////////
// function iola::xml::parser

/// Parse the XML input stream into the root element using the supplied factory.
void parser(ifactory* pkFactory, ielement* pkRoot, std::istream& isXML);

} // namespace xml
} // namespace iola

#endif // IOLA_XML_PARSER_H
