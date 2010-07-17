/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOM_HEIGHT_ELEMENT_H
#define IOLA_DOM_HEIGHT_ELEMENT_H

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

// IOLA
#include "integer_terminal.h"

namespace  iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::height_element

class height_element :
	public integer_terminal
{
public:
	height_element(const std::string strName);
};

} // namespace dom
} // namespace iola

#endif // IOLA_DOM_HEIGHT_ELEMENT_H
