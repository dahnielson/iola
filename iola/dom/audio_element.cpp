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

#include "audio_element.h"
#include "duration_element.h"
#include "rate_element.h"
#include "samplecharacteristics_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::audio_element

audio_element::audio_element(const std::string strName) :
        m_strName(strName),
	m_pkDuration(0),
	m_pkRate(0),
	m_pkSampleCharacteristics(0)
{}

audio_element::~audio_element()
{
	if (m_pkDuration)
		delete m_pkDuration;
	if (m_pkRate)
		delete m_pkRate;
	if (m_pkSampleCharacteristics)
		delete m_pkSampleCharacteristics;
}

void
audio_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
	else if (dynamic_cast<samplecharacteristics_element*>(pkElement))
		m_pkSampleCharacteristics = dynamic_cast<samplecharacteristics_element*>(pkElement);
}

void
audio_element::attribute(std::string strKey, std::string strValue)
{
}

void
audio_element::text(std::string strText)
{
}

void
audio_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	if (m_pkSampleCharacteristics)
		m_pkSampleCharacteristics->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
audio_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	if (m_pkDuration)
		m_pkDuration->accept(visitor);
	if (m_pkRate)
		m_pkRate->accept(visitor);
	if (m_pkSampleCharacteristics)
		m_pkSampleCharacteristics->accept(visitor);
}

void
audio_element::restore(iola::model::iasset* object)
{
	if (m_pkDuration)
		m_pkDuration->restore(object);
	if (m_pkRate)
		m_pkRate->restore(object->video_settings());
	if (m_pkSampleCharacteristics)
		m_pkSampleCharacteristics->restore(object);
}

} // namespace dom
} // namespace iola
