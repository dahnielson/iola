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

#include "string_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::string_element

string_element::string_element(const std::string strName) :
        m_strName(strName)
{
}

void
string_element::child(iola::xml::ielement* pkElement)
{
}

void
string_element::attribute(std::string strKey, std::string strValue)
{
}

void
string_element::text(std::string strText)
{
	if (m_strValue.empty())
		m_strValue = strText;
}

void
string_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_strValue;
	osXML << "</" << m_strName << ">"  << std::endl;
}

void
string_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
}

void
string_element::restore(iola::model::iasset* object)
{
	object->set_timecode(m_strValue);
}

void
string_element::set(const std::string value)
{
	m_strValue = value;
}

} // namespace dom
} // namespace iola
