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

#include "pixelaspectratio_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::pixelaspectratio_element

pixelaspectratio_element::pixelaspectratio_element(const std::string strName) :
        m_strName(strName)
{}

void
pixelaspectratio_element::child(iola::xml::ielement* pkElement)
{
}

void
pixelaspectratio_element::attribute(std::string strKey, std::string strValue)
{
}

void
pixelaspectratio_element::text(std::string strText)
{
	if (m_strValue.empty())
		m_strValue = strText;
}

void
pixelaspectratio_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_strValue;
	osXML << "</" << m_strName << ">"  << std::endl;
}

void
pixelaspectratio_element::restore(iola::model::ivideo_settings* object)
{
	iola::model::ivideo_settings::par_t par;
	if (m_strValue == "SQUARE" || m_strValue == "square")
		par = iola::model::ivideo_settings::SQUARE;
	else if (m_strValue == "NTSC-601")
		par = iola::model::ivideo_settings::NTSC_601;
	else if (m_strValue == "PAL-601")
		par = iola::model::ivideo_settings::PAL_601;
	else if (m_strValue == "HD-(960x720)" || m_strValue == "DVCPROHD-720p")
		par = iola::model::ivideo_settings::HD_960x720;
	else if (m_strValue == "HD-(1280x1080)" || m_strValue == "DVCPROHD-1080i60")
		par = iola::model::ivideo_settings::HD_1280x1080;
	else if (m_strValue == "HD-(1440x1080)" || m_strValue == "DVCPROHD-1080i50")
		par = iola::model::ivideo_settings::HD_1440x1080;
	else
		return;
	object->set_par(par);
}

void
pixelaspectratio_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
}

void
pixelaspectratio_element::set(const iola::model::ivideo_settings::par_t value)
{
	switch (value)
	{
	case iola::model::ivideo_settings::SQUARE:
		m_strValue = "SQUARE";
	case iola::model::ivideo_settings::NTSC_601:
		m_strValue = "NTSC-601";
	case iola::model::ivideo_settings::PAL_601:
		m_strValue = "PAL-601";
	case iola::model::ivideo_settings::HD_960x720:
		m_strValue = "HD-(960x720)";
	case iola::model::ivideo_settings::HD_1280x1080:
		m_strValue = "HD-(1280x1080)";
	case iola::model::ivideo_settings::HD_1440x1080:
		m_strValue = "HD-(1440x1080)";
	default:
		assert(!"Default case should never be reached");
	};
}

} // namespace dom
} // namespace iola
