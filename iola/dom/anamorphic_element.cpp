/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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

#include "anamorphic_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::anamorphic_element

anamorphic_element::anamorphic_element(const std::string strName) :
        m_strName(strName)
{
}

void
anamorphic_element::child(iola::xml::ielement* pkElement)
{
}

void
anamorphic_element::attribute(std::string strKey, std::string strValue)
{
}

void
anamorphic_element::text(std::string strText)
{
	if (strText == "TRUE" || strText == "true" || strText == "True")
		m_bValue = true;
	else if (strText == "FALSE" || strText == "false" || strText == "False")
		m_bValue = false;
}

void
anamorphic_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	if (m_bValue)
		osXML << "TRUE";
	else
		osXML << "FALSE";
	osXML << "</" << m_strName << ">" << std::endl;
}

void
anamorphic_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
}

void
anamorphic_element::restore(iola::model::ivideo_settings* object)
{
	object->set_anamorphic(m_bValue);
}

void
anamorphic_element::set(const bool value)
{
	m_bValue = value;
}

} // namespace dom
} // namespace iola
