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

#include "iola_element.h"
#include "sequence_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::iola_element

iola_element::iola_element(const std::string strName) :
        m_strName(strName),
	m_pkSequence(0)
{}

void
iola_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<sequence_element*>(pkElement))
		m_pkSequence = dynamic_cast<sequence_element*>(pkElement);
}

void
iola_element::attribute(std::string strKey, std::string strValue)
{
	if (strKey == "version")
		m_strVersion = strValue;
}

void
iola_element::text(std::string strText)
{
}

void
iola_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << " version=\"" << m_strVersion << "\">";
	if (m_pkSequence)
		m_pkSequence->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
iola_element::restore()
{
	if (m_pkSequence)
		m_pkSequence->restore();
}

void
iola_element::store()
{
	if (m_pkSequence)
		m_pkSequence->store();
}

} // namespace dom
} // namespace iola
