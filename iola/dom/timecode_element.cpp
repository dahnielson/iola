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

// BOOST
#include <boost/lexical_cast.hpp>

// IOLA
#include <iola/application/get_instance.h>
#include "rate_element.h"
#include "string_element.h"
#include "timecode_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::timecode_element

timecode_element::timecode_element(const std::string strName) :
        m_strName(strName),
	m_pkString(0),
	m_pkRate(0)
{}

void
timecode_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<string_element*>(pkElement))
		m_pkString = dynamic_cast<string_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
}

void
timecode_element::attribute(std::string strKey, std::string strValue)
{
}

void
timecode_element::text(std::string strText)
{
}

void
timecode_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkString)
		m_pkString->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
timecode_element::restore()
{
	if (m_pkString)
	{
		std::string string = m_pkString->get();
		iola::application::get_instance()->get_project()->program_set_start_timecode(string);
	}

	if (m_pkRate)
		m_pkRate->restore();
}

void
timecode_element::store()
{
	if (m_pkString)
	{
		std::string string = iola::application::get_instance()->get_project()->program_get_start_timecode();
		m_pkString->set(string);
	}

	if (m_pkRate)
		m_pkRate->store();
}

} // namespace dom
} // namespace iola
