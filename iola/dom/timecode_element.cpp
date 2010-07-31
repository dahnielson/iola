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

timecode_element::~timecode_element()
{
	if (m_pkString)
		delete m_pkString;
	if (m_pkRate)
		delete m_pkRate;
}

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
	if (strKey == "id")
		m_strID = strValue;
}

void
timecode_element::text(std::string strText)
{
}

void
timecode_element::xml(std::ostream& osXML)
{
	if (m_strID.empty())
		osXML << "<" << m_strName << ">" << std::endl;
	else
		osXML << "<" << m_strName << " id=\"" << m_strID << "\">" << std::endl;
	if (m_pkString)
		m_pkString->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
timecode_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	if (m_pkString)
		m_pkString->accept(visitor);
	if (m_pkRate)
		m_pkRate->accept(visitor);
}

void
timecode_element::restore(iola::model::iasset* object)
{
	if (m_pkString)
		m_pkString->restore(object);
	if (m_pkRate)
		m_pkRate->restore(object->video_settings());
}

} // namespace dom
} // namespace iola
