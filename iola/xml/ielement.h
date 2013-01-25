/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_XML_IELEMENT_H
#define IOLA_XML_IELEMENT_H

// Iola NLE
// Copyright (c) 2010-2013, Anders Dahnielson
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
#include <iostream>

// IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace xml
{

class ivisitor;

////////////////////////////////////////////////////////////////////////////
// class iola::xml::ielement

/// Abstract interface of XML element
class ielement :
	public virtual iola::iunknown
{
public:
	/// Append a child element to the element.
	virtual void child(ielement* pkElement) = 0;
	/// Set an attribute on the element.
	virtual void attribute(std::string strKey, std::string strValue) = 0;
	/// Append CDATA to the element.
	virtual void text(std::string strText) = 0;
	/// Write the element as XML to an output stream.
	virtual void xml(std::ostream& osXML) = 0;
	/// Accept visitor
	virtual void accept(ivisitor* visitor) = 0;

protected:
	ielement()
	{}
	ielement(const ielement& RHS);
	ielement& operator=(const ielement& RHS)
	{
		return *this;
	}
	virtual ~ielement()
	{}
};

} // namespace xml
} // namespace iola

#endif // IOLA_XML_IELEMENT_H
