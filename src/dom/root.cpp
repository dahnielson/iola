/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

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
#include "root.h"
#include "iola_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::root

root::root()
{}

void
root::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<iola_element*>(pkElement))
		m_pkIola = dynamic_cast<iola_element*>(pkElement);
}

void
root::attribute(std::string strKey, std::string strValue)
{
	// The root element ain't got no stinking attributes!
}

void
root::text(std::string strText)
{
	// The root ignores text, find it boring and pointless...
}

void
root::xml(std::ostream& osXML)
{
	osXML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
	osXML << "<!DOCTYPE iola>" << std::endl;
	m_pkIola->xml(osXML);
}

void
root::restore()
{
	m_pkIola->restore();
}

} // namespace dom
} // namespace iola
