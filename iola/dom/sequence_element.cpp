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

#include <iola/application/get_instance.h>

#include "sequence_element.h"
#include "duration_element.h"
#include "media_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::sequence_element

sequence_element::sequence_element(const std::string strName) :
        m_strName(strName),
	m_pkDuration(0),
	m_pkMedia(0)
{}

void
sequence_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<media_element*>(pkElement))
		m_pkMedia = dynamic_cast<media_element*>(pkElement);
}

void
sequence_element::attribute(std::string strKey, std::string strValue)
{
}

void
sequence_element::text(std::string strText)
{
}

void
sequence_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkMedia)
		m_pkMedia->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
sequence_element::restore()
{
	if (m_pkDuration)
	{
		int duration = m_pkDuration->get();
		iola::application::get_instance()->get_project()->program_set_duration(duration);
	}

	if (m_pkMedia)
		m_pkMedia->restore();
}

void
sequence_element::store()
{
	if (m_pkDuration)
	{
		int duration = iola::application::get_instance()->get_project()->program_get_duration();
		m_pkDuration->set(duration);
	}

	if (m_pkMedia)
		m_pkMedia->store();
}

} // namespace dom
} // namespace iola
