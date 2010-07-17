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
#include <iola/application/get_instance.h>
#include <iola/domain/iproject.h>
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
	// No attributes.
}

void
samplecharacteristics_element::text(std::string strText)
{
	// No text.
}

void
samplecharacteristics_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
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
	osXML << "</" << m_strName << ">";
}

void
samplecharacteristics_element::restore()
{
	// Video
	if (m_pkWidth)
	{
		const int width = m_pkWidth->get();
		iola::application::get_instance()->get_project()->set_width(width);
	}

	if (m_pkHeight)
	{
		const int height = m_pkHeight->get();
		iola::application::get_instance()->get_project()->set_height(height);
	}

	if (m_pkAnamorphic)
	{
		const bool anamorphic = m_pkAnamorphic->get();
		iola::application::get_instance()->get_project()->set_anamorphic(anamorphic);
	}

	if (m_pkPixelAspectRatio)
	{
		iola::domain::iproject::par_t par;
		const std::string strPAR = m_pkPixelAspectRatio->get();
		if (strPAR == "SQUARE" || strPAR == "square")
			par = iola::domain::iproject::SQUARE;
		else if (strPAR == "NTSC-601")
			par = iola::domain::iproject::NTSC_601;
		else if (strPAR == "PAL-601")
			par = iola::domain::iproject::PAL_601;
		else if (strPAR == "HD-(960x720)" || strPAR == "DVCPROHD-720p")
			par = iola::domain::iproject::HD_960x720;
		else if (strPAR == "HD-(1280x1080)" || strPAR == "DVCPROHD-1080i60")
			par = iola::domain::iproject::HD_1280x1080;
		else if (strPAR == "HD-(1440x1080)" || strPAR == "DVCPROHD-1080i50")
			par = iola::domain::iproject::HD_1440x1080;
		iola::application::get_instance()->get_project()->set_par(par);
	}

	if (m_pkFieldDominance)
	{
		iola::domain::iproject::field_t field;
		const std::string strField = m_pkFieldDominance->get();
		if (strField == "NONE" || strField == "none")
			field = iola::domain::iproject::NONE;
		else if (strField == "LOWER" || strField == "lower" || strField == "EVEN" || strField == "even")
			field = iola::domain::iproject::EVEN;
		else if (strField == "UPPER" || strField == "upper" || strField == "ODD" || strField == "odd")
			field = iola::domain::iproject::ODD;
		iola::application::get_instance()->get_project()->set_field_dominance(field);
	}

	if (m_pkRate)
		m_pkRate->restore();

	// Audio
	if (m_pkDepth)
	{
		const int depth = m_pkDepth->get();
		switch (depth)
		{
		case 16:
			iola::application::get_instance()->get_project()->set_sample_depth(depth);
			break;
		};
	}

	if (m_pkSampleRate)
	{
		const int rate = m_pkSampleRate->get();
		switch (rate)
		{
		case 32000:
		case 44100:
		case 48000:
			iola::application::get_instance()->get_project()->set_sample_rate(rate);
			break;
		};
	}
}

void
samplecharacteristics_element::store()
{
	// Video
	if (m_pkWidth)
	{
		const int width = iola::application::get_instance()->get_project()->get_width();
		m_pkWidth->set(width);
	}

	if (m_pkHeight)
	{
		const int height = iola::application::get_instance()->get_project()->get_height();
		m_pkHeight->set(height);
	}

	if (m_pkAnamorphic)
	{
		const bool anamorphic = iola::application::get_instance()->get_project()->get_anamorphic();
		m_pkAnamorphic->set(anamorphic);
	}

	if (m_pkPixelAspectRatio)
	{
		iola::domain::iproject::par_t par = iola::application::get_instance()->get_project()->get_par();
		switch (par)
		{
		case iola::domain::iproject::SQUARE:
			m_pkPixelAspectRatio->set("SQUARE");
			break;
		case iola::domain::iproject::NTSC_601:
			m_pkPixelAspectRatio->set("NTSC-601");
			break;
		case iola::domain::iproject::PAL_601:
			m_pkPixelAspectRatio->set("PAL-601");
			break;
		case iola::domain::iproject::HD_960x720:
			m_pkPixelAspectRatio->set("HD-(960x720)");
			break;
		case iola::domain::iproject::HD_1280x1080:
			m_pkPixelAspectRatio->set("HD-(1280x1080)");
			break;
		case iola::domain::iproject::HD_1440x1080:
			m_pkPixelAspectRatio->set("HD-(1440x1080)");
			break;
		};
	}

	if (m_pkFieldDominance)
	{
		iola::domain::iproject::field_t field  = iola::application::get_instance()->get_project()->get_field_dominance();
		switch (field)
		{
		case iola::domain::iproject::NONE:
			m_pkFieldDominance->set("NONE");
			break;
		case iola::domain::iproject::EVEN:
			m_pkFieldDominance->set("EVEN");
			break;
		case iola::domain::iproject::ODD:
			m_pkFieldDominance->set("ODD");
			break;
		};
	}

	if (m_pkRate)
		m_pkRate->store();

	// Audio
	if (m_pkDepth)
	{
		const int depth = iola::application::get_instance()->get_project()->get_sample_depth();
		switch (depth)
		{
		case 16:
			m_pkDepth->set(depth);
			break;
		};
	}

	if (m_pkSampleRate)
	{
		const int rate = iola::application::get_instance()->get_project()->get_sample_rate();
		switch (rate)
		{
		case 32000:
		case 44100:
		case 48000:
			m_pkSampleRate->set(rate);
			break;
		};
	}
}

} // namespace dom
} // namespace iola
