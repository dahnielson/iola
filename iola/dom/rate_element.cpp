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
#include "ntsc_element.h"
#include "rate_element.h"
#include "timebase_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::rate_element

rate_element::rate_element(const std::string strName) :
        m_strName(strName),
	m_pkTimebase(0),
	m_pkNTSC(0)
{}

void
rate_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<timebase_element*>(pkElement))
		m_pkTimebase = dynamic_cast<timebase_element*>(pkElement);
	else if (dynamic_cast<ntsc_element*>(pkElement))
		m_pkNTSC = dynamic_cast<ntsc_element*>(pkElement);
}

void
rate_element::attribute(std::string strKey, std::string strValue)
{
}

void
rate_element::text(std::string strText)
{
}

void
rate_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkTimebase)
		m_pkTimebase->xml(osXML);
	if (m_pkNTSC)
		m_pkNTSC->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
rate_element::restore(iola::model::ivideo_settings* object)
{
	if (m_pkTimebase)
		m_pkTimebase->restore(object);
	if (m_pkNTSC)
		m_pkNTSC->restore(object);
}

void
rate_element::store(ivisitor* visitor)
{
	if (m_pkTimebase)
		m_pkTimebase->store(visitor);
	if (m_pkNTSC)
		m_pkNTSC->store(visitor);
}

} // namespace dom
} // namespace iola
