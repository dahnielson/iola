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
#include "name_element.h"
#include "rate_element.h"
#include "timecode_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::sequence_element

sequence_element::sequence_element(const std::string strName) :
        m_strName(strName),
	m_pkName(0),
	m_pkDuration(0),
	m_pkRate(0),
	m_pkTimecode(0),
	m_pkMedia(0)
{}

void
sequence_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<name_element*>(pkElement))
		m_pkName = dynamic_cast<name_element*>(pkElement);
	else if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
	else if (dynamic_cast<timecode_element*>(pkElement))
		m_pkTimecode = dynamic_cast<timecode_element*>(pkElement);
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
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkName)
		m_pkName->xml(osXML);
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	if (m_pkTimecode)
		m_pkTimecode->xml(osXML);
	if (m_pkMedia)
		m_pkMedia->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
sequence_element::restore()
{
	if (m_pkName)
	{
		std::string name = m_pkName->get();
		iola::application::get_instance()->get_project()->program_set_name(name);
	}

	if (m_pkDuration)
	{
		int duration = m_pkDuration->get();
		iola::application::get_instance()->get_project()->program_set_duration(duration);
	}

	if (m_pkRate)
		m_pkRate->restore();

	if (m_pkTimecode)
		m_pkTimecode->restore();

	if (m_pkMedia)
		m_pkMedia->restore();
}

void
sequence_element::store()
{
	if (m_pkName)
	{
		std::string name = iola::application::get_instance()->get_project()->program_get_name();
		m_pkName->set(name);
	}

	if (m_pkDuration)
	{
		int duration = iola::application::get_instance()->get_project()->program_get_duration();
		m_pkDuration->set(duration);
	}

	if (m_pkRate)
		m_pkRate->store();

	if (m_pkTimecode)
		m_pkTimecode->store();

	if (m_pkMedia)
		m_pkMedia->store();
}

} // namespace dom
} // namespace iola
