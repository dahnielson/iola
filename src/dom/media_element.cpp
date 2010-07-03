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

#include "media_element.h"
#include "video_element.h"
#include "audio_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::media_element

media_element::media_element(const std::string strName) :
        m_strName(strName),
	m_pkVideo(0),
	m_pkAudio(0)
{}

void
media_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<video_element*>(pkElement))
		m_pkVideo = dynamic_cast<video_element*>(pkElement);
	else if (dynamic_cast<audio_element*>(pkElement))
		m_pkAudio = dynamic_cast<audio_element*>(pkElement);
}

void
media_element::attribute(std::string strKey, std::string strValue)
{
}

void
media_element::text(std::string strText)
{
}

void
media_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	if (m_pkVideo)
		m_pkVideo->xml(osXML);
	if (m_pkAudio)
		m_pkAudio->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
media_element::restore()
{
	if (m_pkVideo)
		m_pkVideo->restore();
	if (m_pkAudio)
		m_pkAudio->restore();
}

void
media_element::store()
{
	if (m_pkVideo)
		m_pkVideo->store();
	if (m_pkAudio)
		m_pkAudio->store();
}

} // namespace dom
} // namespace iola
