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

#include "clipitem_element.h"
#include "end_element.h"
#include "in_element.h"
#include "out_element.h"
#include "pathurl_element.h"
#include "start_element.h"
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
	osXML << "<" << m_strName << ">";
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
		(*i)->xml(osXML);
	osXML << "</" << m_strName << ">";
}

void
track_element::restore()
{
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
		(*i)->restore();
}

void
track_element::store()
{
	// Erase all clip items
	m_vpkClipItem.erase(m_vpkClipItem.begin(), m_vpkClipItem.end());

	// Add clip items
	const int number_of_clips = iola::application::get_instance()->get_project()->program_get_clip_count();
	for (int i = 0; i < number_of_clips; ++i)
	{
		Mlt::ClipInfo* pkInfo = iola::application::get_instance()->get_project()->program_get_clip_info(i);

		clipitem_element* pkClipItem = new clipitem_element("clipitem");
		pathurl_element* pkPathURL = new pathurl_element("pathurl");
		in_element* pkIn = new in_element("in");
		out_element* pkOut = new out_element("out");
		start_element* pkStart = new start_element("start");
		end_element* pkEnd = new end_element("end");

		pkPathURL->set(pkInfo->resource);
		pkIn->set(pkInfo->frame_in);
		pkOut->set(pkInfo->frame_out);
		pkStart->set(pkInfo->start);
		pkEnd->set(pkInfo->start + pkInfo->frame_out - pkInfo->frame_in);

		pkClipItem->child(pkPathURL);
		pkClipItem->child(pkIn);
		pkClipItem->child(pkOut);
		pkClipItem->child(pkStart);
		pkClipItem->child(pkEnd);

		m_vpkClipItem.push_back(pkClipItem);

		Mlt::Playlist::delete_clip_info(pkInfo);
	}

	// Call all children
	for (clipitem_iterator_t i = m_vpkClipItem.begin(); i != m_vpkClipItem.end(); i++)
		(*i)->store();
}

} // namespace dom
} // namespace iola
