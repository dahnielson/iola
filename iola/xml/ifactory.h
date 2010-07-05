/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_XML_IFACTORY_H
#define IOLA_XML_IFACTORY_H

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
#include <string>

// IOLA
#include <iola/iunknown.h>
#include "ielement.h"

namespace iola
{
namespace xml
{

////////////////////////////////////////////////////////////////////////////
// class iola::xml::ifactory

class ifactory :
	public virtual iola::iunknown
{
public:
	/// Return a element instance matching the supplied element name.
	virtual ielement* factory(const std::string strElement) = 0;

protected:
	ifactory()
	{}
	ifactory(const ifactory& RHS);
	ifactory& operator=(const ifactory& RHS)
	{
		return *this;
	}
	virtual ~ifactory()
	{}
};

} // namespace xml
} // namespace iola

#endif // IOLA_XML_IFACTORY_H
