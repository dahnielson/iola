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

#include "fielddominance_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::fielddominance_element

fielddominance_element::fielddominance_element(const std::string strName) :
        m_strName(strName)
{
}

void
fielddominance_element::child(iola::xml::ielement* pkElement)
{
}

void
fielddominance_element::attribute(std::string strKey, std::string strValue)
{
}

void
fielddominance_element::text(std::string strText)
{
	if (m_strValue.empty())
		m_strValue = strText;
}

void
fielddominance_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_strValue;
	osXML << "</" << m_strName << ">"  << std::endl;
}

void
fielddominance_element::restore(iola::model::ivideo_settings* object)
{
	iola::model::ivideo_settings::field_t field;
	if (m_strValue == "NONE" || m_strValue == "none")
		field = iola::model::ivideo_settings::NONE;
	else if (m_strValue == "LOWER" || m_strValue == "lower" || m_strValue == "EVEN" || m_strValue == "even")
		field = iola::model::ivideo_settings::EVEN;
	else if (m_strValue == "UPPER" || m_strValue == "upper" || m_strValue == "ODD" || m_strValue == "odd")
		field = iola::model::ivideo_settings::ODD;
	else
		return;
	object->set_field_dominance(field);
}

void
fielddominance_element::store(ivisitor* visitor)
{
	visitor->visit(this);
}

void
fielddominance_element::set(const iola::model::ivideo_settings::field_t value)
{
	switch (value)
	{
	case iola::model::ivideo_settings::NONE:
		m_strValue = "NONE";
	case iola::model::ivideo_settings::EVEN:
		m_strValue = "EVEN";
	case iola::model::ivideo_settings::ODD:
		m_strValue = "ODD";
	default:
		assert(!"Default case should never be reached");
	};
}

} // namespace dom
} // namespace iola
