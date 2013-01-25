/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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

#include "name_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::name_element

name_element::name_element(const std::string strName) :
        m_strName(strName)
{
}

void
name_element::child(iola::xml::ielement* pkElement)
{
}

void
name_element::attribute(std::string strKey, std::string strValue)
{
}

void
name_element::text(std::string strText)
{
	if (m_strValue.empty())
		m_strValue = strText;
}

void
name_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_strValue;
	osXML << "</" << m_strName << ">"  << std::endl;
}

void
name_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
}

void
name_element::restore(iola::model::iasset* object)
{
	object->set_name(m_strValue);
}

void
name_element::set(const std::string value)
{
	m_strValue = value;
}

} // namespace dom
} // namespace iola
