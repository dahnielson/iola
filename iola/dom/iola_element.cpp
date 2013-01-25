/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

// Iola NLE
// Copyright (c) 2010-2013, Anders Dahnielson
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

#include <iola/model/iclip.h>
#include <iola/model/isequence.h>
#include "clip_element.h"
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
	m_pkClip(0),
	m_pkSequence(0)
{}

iola_element::~iola_element()
{
	if (m_pkSequence)
		delete m_pkSequence;
}

void
iola_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<clip_element*>(pkElement))
		m_pkClip = dynamic_cast<clip_element*>(pkElement);
	else if (dynamic_cast<sequence_element*>(pkElement))
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
	osXML << "<" << m_strName << " version=\"" << m_strVersion << "\">" << std::endl;
	if (m_pkClip)
		m_pkClip->xml(osXML);
	if (m_pkSequence)
		m_pkSequence->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
iola_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	if (m_pkSequence)
		m_pkSequence->accept(visitor);
}

void
iola_element::restore(iola::iunknown* object)
{
	if (m_pkClip)
	{
		iola::model::iclip* clip = dynamic_cast<iola::model::iclip*>(object);
		if (clip)
			m_pkClip->restore(clip);
		return;
	}

	if (m_pkSequence)
	{
		iola::model::isequence* sequence = dynamic_cast<iola::model::isequence*>(object);
		if (sequence)
			m_pkSequence->restore(sequence);
		return;
	}
}

} // namespace dom
} // namespace iola
