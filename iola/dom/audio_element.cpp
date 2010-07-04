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

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::audio_element

audio_element::audio_element(const std::string strName) :
        m_strName(strName)
{}

void
audio_element::child(iola::xml::ielement* pkElement)
{
	// Append any legitimate children...
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
	osXML << "<" << m_strName << ">";
	// Call xml(osXML) on any children...
	osXML << "</" << m_strName << ">";
}

void
audio_element::restore()
{
	// Call restore() on any children...
}

void
audio_element::store()
{
	// Call store() on any children...
}

} // namespace dom
} // namespace iola
