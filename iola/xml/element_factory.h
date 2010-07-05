/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_XML_ELEMENT_FACTORY_H
#define IOLA_XML_ELEMENT_FACTORY_H

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
#include "ielement.h"
#include "ifactory.h"

namespace iola
{
namespace xml
{

////////////////////////////////////////////////////////////////////////////
// class iola::xml::element_factory

class element_factory :
	public ifactory
{
public:
	ielement* factory(const std::string strElement);
};

} // namespace xml
} // namespace iola

#endif // IOLA_XML_ELEMENT_FACTORY_H
