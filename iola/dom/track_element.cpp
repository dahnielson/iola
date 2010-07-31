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
#include <iola/model/iclip.h>

#include <iola/model/clip.h>
#include "clipitem_element.h"
#include "track_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::track_element

track_element::track_element(const std::string strName) :
        m_strName(strName)
{}

track_element::~track_element()
{
}

void
track_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<clipitem_element*>(pkElement))
		m_vpkClipItem.push_back(dynamic_cast<clipitem_element*>(pkElement));
}

void
track_element::attribute(std::string strKey, std::string strValue)
{
}

void
track_element::text(std::string strText)
{
}

void
track_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
		(*i)->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
track_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
		(*i)->accept(visitor);
}

void
track_element::restore(iola::model::isequence* object)
{
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
	{
		iola::model::iclip* clipitem = iola::model::create_clip();
		(*i)->restore(clipitem);
		object->cut_region(clipitem->markers());
		object->insert_clip(clipitem);
		delete clipitem;
	}
}

} // namespace dom
} // namespace iola
