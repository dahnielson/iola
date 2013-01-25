/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOM_CLIP_ELEMENT_H
#define IOLA_DOM_CLIP_ELEMENT_H

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

// IOLA
#include <iola/model/iclip.h>
#include <iola/xml/ielement.h>
#include <iola/xml/ivisitor.h>

namespace  iola
{
namespace dom
{

class anamorphic_element;
class duration_element;
class end_element;
class file_element;
class fielddominance_element;
class in_element;
class name_element;
class out_element;
class pixelaspectratio_element;
class rate_element;
class start_element;
class timecode_element;

////////////////////////////////////////////////////////////////////////////
// class iola::dom::clip_element

class clip_element :
	public iola::xml::ielement
{
public:
	clip_element(const std::string strName);
	~clip_element();
	void child(iola::xml::ielement* pkElement);
	void attribute(std::string strKey, std::string strValue);
	void text(std::string strText);
	void xml(std::ostream& osXML);
	void accept(iola::xml::ivisitor* visitor);

	void restore(iola::model::iclip* object);

private:
	const std::string m_strName;
	std::string m_strID;
	name_element* m_pkName;
	file_element* m_pkFile;
	duration_element* m_pkDuration;
	in_element* m_pkIn;
	out_element* m_pkOut;
	start_element* m_pkStart;
	end_element* m_pkEnd;
	rate_element* m_pkRate;
	timecode_element* m_pkTimecode;
	anamorphic_element* m_pkAnamorphic;
	pixelaspectratio_element* m_pkPixelAspectRatio;
	fielddominance_element* m_pkFieldDominance;
};

} // namespace dom
} // namespace iola

#endif // IOLA_DOM_CLIP_ELEMENT_H
