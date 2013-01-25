/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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
#include <iola/model/ivideo_settings.h>

#include "anamorphic_element.h"
#include "depth_element.h"
#include "fielddominance_element.h"
#include "height_element.h"
#include "pixelaspectratio_element.h"
#include "rate_element.h"
#include "samplecharacteristics_element.h"
#include "samplerate_element.h"
#include "width_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::samplecharacteristics_element

samplecharacteristics_element::samplecharacteristics_element(const std::string strName) :
        m_strName(strName),
	m_pkWidth(0),
	m_pkHeight(0),
	m_pkAnamorphic(0),
	m_pkPixelAspectRatio(0),
	m_pkFieldDominance(0),
	m_pkRate(0),
	m_pkDepth(0),
	m_pkSampleRate(0)
{}

samplecharacteristics_element::~samplecharacteristics_element()
{
	// Video
	if (m_pkWidth)
		delete m_pkWidth;
	if (m_pkHeight)
		delete m_pkHeight;
	if (m_pkAnamorphic)
		delete m_pkAnamorphic;
	if (m_pkPixelAspectRatio)
		delete m_pkPixelAspectRatio;
	if (m_pkFieldDominance)
		delete m_pkFieldDominance;
	if (m_pkRate)
		delete m_pkRate;
	// Audio
	if (m_pkDepth)
		delete m_pkDepth;
	if (m_pkSampleRate)
		delete m_pkSampleRate;
}

void
samplecharacteristics_element::child(iola::xml::ielement* pkElement)
{
	// Video
	if (dynamic_cast<width_element*>(pkElement))
		m_pkWidth = dynamic_cast<width_element*>(pkElement);
	else if (dynamic_cast<height_element*>(pkElement))
		m_pkHeight = dynamic_cast<height_element*>(pkElement);
	else if (dynamic_cast<anamorphic_element*>(pkElement))
		m_pkAnamorphic = dynamic_cast<anamorphic_element*>(pkElement);
	else if (dynamic_cast<pixelaspectratio_element*>(pkElement))
		m_pkPixelAspectRatio = dynamic_cast<pixelaspectratio_element*>(pkElement);
	else if (dynamic_cast<fielddominance_element*>(pkElement))
		m_pkFieldDominance = dynamic_cast<fielddominance_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
	// Audio
	else if (dynamic_cast<depth_element*>(pkElement))
		m_pkDepth = dynamic_cast<depth_element*>(pkElement);
	else if (dynamic_cast<samplerate_element*>(pkElement))
		m_pkSampleRate = dynamic_cast<samplerate_element*>(pkElement);
}

void
samplecharacteristics_element::attribute(std::string strKey, std::string strValue)
{
}

void
samplecharacteristics_element::text(std::string strText)
{
}

void
samplecharacteristics_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	// Video
	if (m_pkWidth)
		m_pkWidth->xml(osXML);
	if (m_pkHeight)
		m_pkHeight->xml(osXML);
	if (m_pkAnamorphic)
		m_pkAnamorphic->xml(osXML);
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->xml(osXML);
	if (m_pkFieldDominance)
		m_pkFieldDominance->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	// Audio
	if (m_pkDepth)
		m_pkDepth->xml(osXML);
	if (m_pkSampleRate)
		m_pkSampleRate->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
samplecharacteristics_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	// Video
	if (m_pkWidth)
		m_pkWidth->accept(visitor);
	if (m_pkHeight)
		m_pkHeight->accept(visitor);
	if (m_pkAnamorphic)
		m_pkAnamorphic->accept(visitor);
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->accept(visitor);
	if (m_pkFieldDominance)
		m_pkFieldDominance->accept(visitor);
	if (m_pkRate)
		m_pkRate->accept(visitor);
	// Audio
	if (m_pkDepth)
		m_pkDepth->accept(visitor);
	if (m_pkSampleRate)
		m_pkSampleRate->accept(visitor);
}

void
samplecharacteristics_element::restore(iola::model::iasset* object)
{
	// Video
	if (m_pkWidth)
		m_pkWidth->restore(object->video_settings());
	if (m_pkHeight)
		m_pkWidth->restore(object->video_settings());
	if (m_pkAnamorphic)
		m_pkAnamorphic->restore(object->video_settings());
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->restore(object->video_settings());
	if (m_pkFieldDominance)
		m_pkFieldDominance->restore(object->video_settings());
	if (m_pkRate)
		m_pkRate->restore(object->video_settings());
	// Audio
	if (m_pkDepth)
		m_pkDepth->restore(object->audio_settings());
	if (m_pkSampleRate)
		m_pkSampleRate->restore(object->audio_settings());
}

} // namespace dom
} // namespace iola
