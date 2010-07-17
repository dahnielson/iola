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
#include <iola/application/get_instance.h>
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
	// No attributes.
}

void
rate_element::text(std::string strText)
{
	// No text.
}

void
rate_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	if (m_pkTimebase)
		m_pkTimebase->xml(osXML);
	if (m_pkNTSC)
		m_pkNTSC->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
rate_element::restore()
{
	if (m_pkTimebase)
	{
		const int timebase = m_pkTimebase->get();
		iola::application::get_instance()->get_project()->set_fps_timebase(timebase);
	}

	if (m_pkNTSC)
	{
		const bool ntsc = m_pkNTSC->get();
		iola::application::get_instance()->get_project()->set_fps_ntsc(ntsc);
	}
}

void
rate_element::store()
{
	if (m_pkTimebase)
	{
		const int timebase = iola::application::get_instance()->get_project()->get_fps_timebase();
		m_pkTimebase->set(timebase);
	}

	if (m_pkNTSC)
	{
		const bool ntsc = iola::application::get_instance()->get_project()->get_fps_ntsc();
		m_pkNTSC->set(ntsc);
	}
}

} // namespace dom
} // namespace iola
