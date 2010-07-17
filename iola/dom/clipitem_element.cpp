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
#include <boost/filesystem.hpp>

// IOLA
#include "clipitem_element.h"
#include "end_element.h"
#include "pathurl_element.h"
#include "in_element.h"
#include "out_element.h"
#include "start_element.h"

#include <iola/application/get_instance.h>

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::clipitem_element

clipitem_element::clipitem_element(const std::string strName) :
        m_strName(strName),
	m_pkPathURL(0),
	m_pkIn(0),
	m_pkOut(0),
	m_pkStart(0),
	m_pkEnd(0)
{}

void
clipitem_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<pathurl_element*>(pkElement))
		m_pkPathURL = dynamic_cast<pathurl_element*>(pkElement);
	else if (dynamic_cast<in_element*>(pkElement))
		m_pkIn = dynamic_cast<in_element*>(pkElement);
	else if (dynamic_cast<out_element*>(pkElement))
		m_pkOut = dynamic_cast<out_element*>(pkElement);
	else if (dynamic_cast<start_element*>(pkElement))
		m_pkStart = dynamic_cast<start_element*>(pkElement);
	else if (dynamic_cast<end_element*>(pkElement))
		m_pkEnd = dynamic_cast<end_element*>(pkElement);
}

void
clipitem_element::attribute(std::string strKey, std::string strValue)
{
	// No attributes.
}

void
clipitem_element::text(std::string strText)
{
	// No text.
}

void
clipitem_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkPathURL)
		m_pkPathURL->xml(osXML);
	if (m_pkIn)
		m_pkIn->xml(osXML);
	if (m_pkOut)
		m_pkOut->xml(osXML);
	if (m_pkStart)
		m_pkStart->xml(osXML);
	if (m_pkEnd)
		m_pkEnd->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
clipitem_element::restore()
{
	if (m_pkPathURL && m_pkIn && m_pkOut && m_pkStart && m_pkEnd)
	{
		boost::filesystem::path pathurl(m_pkPathURL->get());
		const int in = m_pkIn->get();
		const int out = m_pkOut->get();
		const int start = m_pkStart->get();
		const int end = m_pkEnd->get();
		iola::application::get_instance()->get_project()->program_overwrite(pathurl, start, in, out);
	}
}

void
clipitem_element::store()
{
	// Has currently only terminal elements
}

} // namespace dom
} // namespace iola
