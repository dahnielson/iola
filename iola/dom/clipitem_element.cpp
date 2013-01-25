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

// BOOST
#include <boost/filesystem.hpp>

// IOLA
#include "anamorphic_element.h"
#include "clipitem_element.h"
#include "duration_element.h"
#include "end_element.h"
#include "file_element.h"
#include "fielddominance_element.h"
#include "in_element.h"
#include "name_element.h"
#include "out_element.h"
#include "pixelaspectratio_element.h"
#include "rate_element.h"
#include "start_element.h"
#include "timecode_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::clipitem_element

clipitem_element::clipitem_element(const std::string strName) :
        m_strName(strName),
	m_pkName(0),
	m_pkDuration(0),
	m_pkFile(0),
	m_pkIn(0),
	m_pkOut(0),
	m_pkStart(0),
	m_pkEnd(0),
	m_pkRate(0),
	m_pkTimecode(0),
	m_pkAnamorphic(0),
	m_pkPixelAspectRatio(0),
	m_pkFieldDominance(0)
{}

clipitem_element::~clipitem_element()
{
	if (m_pkName)
		delete m_pkName;
	if (m_pkDuration)
		delete m_pkDuration;
	if (m_pkFile)
		delete m_pkFile;
	if (m_pkIn)
		delete m_pkIn;
	if (m_pkOut)
		delete m_pkOut;
	if (m_pkStart)
		delete m_pkStart;
	if (m_pkEnd)
		delete m_pkEnd;
	if (m_pkRate)
		delete m_pkRate;
	if (m_pkTimecode)
		delete m_pkTimecode;
	if (m_pkAnamorphic)
		delete m_pkAnamorphic;
	if (m_pkPixelAspectRatio)
		delete m_pkPixelAspectRatio;
	if (m_pkFieldDominance)
		delete m_pkFieldDominance;
}

void
clipitem_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<name_element*>(pkElement))
		m_pkName = dynamic_cast<name_element*>(pkElement);
	else if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<file_element*>(pkElement))
		m_pkFile = dynamic_cast<file_element*>(pkElement);
	else if (dynamic_cast<in_element*>(pkElement))
		m_pkIn = dynamic_cast<in_element*>(pkElement);
	else if (dynamic_cast<out_element*>(pkElement))
		m_pkOut = dynamic_cast<out_element*>(pkElement);
	else if (dynamic_cast<start_element*>(pkElement))
		m_pkStart = dynamic_cast<start_element*>(pkElement);
	else if (dynamic_cast<end_element*>(pkElement))
		m_pkEnd = dynamic_cast<end_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		 m_pkRate = dynamic_cast<rate_element*>(pkElement);
	else if (dynamic_cast<timecode_element*>(pkElement))
		 m_pkTimecode = dynamic_cast<timecode_element*>(pkElement);
	else if (dynamic_cast<anamorphic_element*>(pkElement))
		 m_pkAnamorphic = dynamic_cast<anamorphic_element*>(pkElement);
	else if (dynamic_cast<pixelaspectratio_element*>(pkElement))
		 m_pkPixelAspectRatio = dynamic_cast<pixelaspectratio_element*>(pkElement);
	else if (dynamic_cast<fielddominance_element*>(pkElement))
		 m_pkFieldDominance = dynamic_cast<fielddominance_element*>(pkElement);
}

void
clipitem_element::attribute(std::string strKey, std::string strValue)
{
	if (strKey == "id")
		m_strID = strValue;
}

void
clipitem_element::text(std::string strText)
{
}

void
clipitem_element::xml(std::ostream& osXML)
{
	if (m_strID.empty())
		osXML << "<" << m_strName << ">" << std::endl;
	else
		osXML << "<" << m_strName << " id=\"" << m_strID << "\">" << std::endl;
	if (m_pkName)
		m_pkName->xml(osXML);
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkFile)
		m_pkFile->xml(osXML);
	if (m_pkIn)
		m_pkIn->xml(osXML);
	if (m_pkOut)
		m_pkOut->xml(osXML);
	if (m_pkStart)
		m_pkStart->xml(osXML);
	if (m_pkEnd)
		m_pkEnd->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	if (m_pkTimecode)
		m_pkTimecode->xml(osXML);
	if (m_pkAnamorphic)
		m_pkAnamorphic->xml(osXML);
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->xml(osXML);
	if (m_pkFieldDominance)
		m_pkFieldDominance->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
clipitem_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	if (m_pkName)
		m_pkName->accept(visitor);
	if (m_pkDuration)
		m_pkDuration->accept(visitor);
	if (m_pkFile)
		m_pkFile->accept(visitor);
	if (m_pkIn)
		m_pkIn->accept(visitor);
	if (m_pkOut)
		m_pkOut->accept(visitor);
	if (m_pkStart)
		m_pkStart->accept(visitor);
	if (m_pkEnd)
		m_pkEnd->accept(visitor);
	if (m_pkRate)
		m_pkRate->accept(visitor);
	if (m_pkTimecode)
		m_pkTimecode->accept(visitor);
	if (m_pkAnamorphic)
		m_pkAnamorphic->accept(visitor);
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->accept(visitor);
	if (m_pkFieldDominance)
		m_pkFieldDominance->accept(visitor);
}

void
clipitem_element::restore(iola::model::iclip* object)
{
	if (m_pkName)
		m_pkName->restore(object);
	if (m_pkDuration)
		m_pkDuration->restore(object);
	if (m_pkFile)
		m_pkFile->restore(object);
	if (m_pkIn)
		m_pkIn->restore(object->markers());
	if (m_pkOut)
		m_pkOut->restore(object->markers());
	if (m_pkStart)
		m_pkStart->restore(object->markers());
	if (m_pkEnd)
		m_pkEnd->restore(object->markers());
	if (m_pkRate)
		m_pkRate->restore(object->video_settings());
	if (m_pkTimecode)
		m_pkTimecode->restore(object);
	if (m_pkAnamorphic)
		m_pkAnamorphic->restore(object->video_settings());
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->restore(object->video_settings());
	if (m_pkFieldDominance)
		m_pkFieldDominance->restore(object->video_settings());
}

} // namespace dom
} // namespace iola
