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
#include <iola/model/iproject.h>
#include "duration_element.h"
#include "in_element.h"
#include "media_element.h"
#include "name_element.h"
#include "out_element.h"
#include "pixelaspectratio_element.h"
#include "rate_element.h"
#include "sequence_element.h"
#include "timecode_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::sequence_element

sequence_element::sequence_element(const std::string strName) :
        m_strName(strName),
	m_pkName(0),
	m_pkDuration(0),
	m_pkRate(0),
	m_pkPixelAspectRatio(0),
	m_pkTimecode(0),
	m_pkIn(0),
	m_pkOut(0),
	m_pkMedia(0)
{}

void
sequence_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<name_element*>(pkElement))
		m_pkName = dynamic_cast<name_element*>(pkElement);
	else if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
	else if (dynamic_cast<pixelaspectratio_element*>(pkElement))
		m_pkPixelAspectRatio = dynamic_cast<pixelaspectratio_element*>(pkElement);
	else if (dynamic_cast<timecode_element*>(pkElement))
		m_pkTimecode = dynamic_cast<timecode_element*>(pkElement);
	else if (dynamic_cast<in_element*>(pkElement))
		m_pkIn = dynamic_cast<in_element*>(pkElement);
	else if (dynamic_cast<out_element*>(pkElement))
		m_pkOut = dynamic_cast<out_element*>(pkElement);
	else if (dynamic_cast<media_element*>(pkElement))
		m_pkMedia = dynamic_cast<media_element*>(pkElement);
}

void
sequence_element::attribute(std::string strKey, std::string strValue)
{
	if (strKey == "id")
		m_strID = strValue;
}

void
sequence_element::text(std::string strText)
{
}

void
sequence_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">" << std::endl;
	if (m_pkName)
		m_pkName->xml(osXML);
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	if (m_pkPixelAspectRatio)
		m_pkPixelAspectRatio->xml(osXML);
	if (m_pkIn)
		m_pkIn->xml(osXML);
	if (m_pkOut)
		m_pkOut->xml(osXML);
	if (m_pkTimecode)
		m_pkTimecode->xml(osXML);
	if (m_pkMedia)
		m_pkMedia->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
sequence_element::restore()
{
	if (m_pkName)
	{
		std::string name = m_pkName->get();
		iola::application::get_instance()->get_project()->program_set_name(name);
	}

	if (m_pkDuration)
	{
		int duration = m_pkDuration->get();
		iola::application::get_instance()->get_project()->program_set_duration(duration);
	}

	if (m_pkRate)
		m_pkRate->restore();

	if (m_pkPixelAspectRatio)
	{
		iola::model::iproject::par_t par;
		const std::string strPAR = m_pkPixelAspectRatio->get();
		if (strPAR == "SQUARE" || strPAR == "square")
			par = iola::model::iproject::SQUARE;
		else if (strPAR == "NTSC-601")
			par = iola::model::iproject::NTSC_601;
		else if (strPAR == "PAL-601")
			par = iola::model::iproject::PAL_601;
		else if (strPAR == "HD-(960x720)" || strPAR == "DVCPROHD-720p")
			par = iola::model::iproject::HD_960x720;
		else if (strPAR == "HD-(1280x1080)" || strPAR == "DVCPROHD-1080i60")
			par = iola::model::iproject::HD_1280x1080;
		else if (strPAR == "HD-(1440x1080)" || strPAR == "DVCPROHD-1080i50")
			par = iola::model::iproject::HD_1440x1080;
		iola::application::get_instance()->get_project()->set_par(par);
	}

	if (m_pkIn)
	{
		int in = m_pkIn->get();
		iola::application::get_instance()->get_project()->program_set_mark_in(in);
	}

	if (m_pkOut)
	{
		int out = m_pkOut->get();
		iola::application::get_instance()->get_project()->program_set_mark_out(out);
	}

	if (m_pkTimecode)
		m_pkTimecode->restore();

	if (m_pkMedia)
		m_pkMedia->restore();
}

void
sequence_element::store()
{
	if (m_pkName)
	{
		std::string name = iola::application::get_instance()->get_project()->program_get_name();
		m_pkName->set(name);
	}

	if (m_pkDuration)
	{
		int duration = iola::application::get_instance()->get_project()->program_get_duration();
		m_pkDuration->set(duration);
	}

	if (m_pkRate)
		m_pkRate->store();

	if (m_pkPixelAspectRatio)
	{
		iola::model::iproject::par_t par = iola::application::get_instance()->get_project()->get_par();
		switch (par)
		{
		case iola::model::iproject::SQUARE:
			m_pkPixelAspectRatio->set("SQUARE");
			break;
		case iola::model::iproject::NTSC_601:
			m_pkPixelAspectRatio->set("NTSC-601");
			break;
		case iola::model::iproject::PAL_601:
			m_pkPixelAspectRatio->set("PAL-601");
			break;
		case iola::model::iproject::HD_960x720:
			m_pkPixelAspectRatio->set("HD-(960x720)");
			break;
		case iola::model::iproject::HD_1280x1080:
			m_pkPixelAspectRatio->set("HD-(1280x1080)");
			break;
		case iola::model::iproject::HD_1440x1080:
			m_pkPixelAspectRatio->set("HD-(1440x1080)");
			break;
		};
	}

	if (m_pkTimecode)
		m_pkTimecode->store();

	if (m_pkIn)
	{
		int in = iola::application::get_instance()->get_project()->program_get_mark_in();
		m_pkIn->set(in);
	}

	if (m_pkOut)
	{
		int out = iola::application::get_instance()->get_project()->program_get_mark_out();
		m_pkOut->set(out);
	}

	if (m_pkMedia)
		m_pkMedia->store();
}

} // namespace dom
} // namespace iola
