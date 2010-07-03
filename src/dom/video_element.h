/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOM_VIDEO_ELEMENT_H
#define IOLA_DOM_VIDEO_ELEMENT_H

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
#include "../xml/ielement.h"

namespace  iola
{
namespace dom
{

class track_element;

////////////////////////////////////////////////////////////////////////////
// class iola::dom::video_element

class video_element :
	public iola::xml::ielement
{
public:
	video_element(const std::string strName);
	void child(iola::xml::ielement* pkElement);
	void attribute(std::string strKey, std::string strValue);
	void text(std::string strText);
	void xml(std::ostream& osXML);
	void restore();
	void store();

private:
	const std::string m_strName; 
	track_element* m_pkTrack;
};

} // namespace dom
} // namespace iola

#endif // IOLA_DOM_VIDEO_ELEMENT_H
