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
#include "pathurl_element.h"
#include "in_element.h"
#include "out_element.h"
#include "start_element.h"

#include "../MainWindow.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::clipitem_element

clipitem_element::clipitem_element(const std::string strName) :
        m_strName(strName)
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
	osXML << "<" << m_strName << ">";
	m_pkPathURL->xml(osXML);
	m_pkIn->xml(osXML);
	m_pkOut->xml(osXML);
	m_pkStart->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
clipitem_element::restore()
{
	boost::filesystem::path pathurl(m_pkPathURL->get());
	const int in = m_pkIn->get();
	const int out = m_pkOut->get();
	const int start = m_pkStart->get();
	iola::application::factory()->program_overwrite(pathurl, start, in, out);
}

} // namespace dom
} // namespace iola
