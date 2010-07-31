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

// BOOST
#include <boost/lexical_cast.hpp>

// IOLA
#include "width_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::width_element

width_element::width_element(const std::string strName) :
        m_strName(strName),
	m_iValue(0)
{
}

void
width_element::child(iola::xml::ielement* pkElement)
{
}

void
width_element::attribute(std::string strKey, std::string strValue)
{
}

void
width_element::text(std::string strText)
{
	try
	{
		m_iValue = boost::lexical_cast<int>(strText);
	}
	catch(boost::bad_lexical_cast &)
        {
		m_iValue = 0;
	}
}

void
width_element::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_iValue;
	osXML << "</" << m_strName << ">" << std::endl;
}

void
width_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
}

void
width_element::restore(iola::model::ivideo_settings* object)
{
	object->set_height(m_iValue);
}

void
width_element::set(const int value)
{
	m_iValue = value;
}

} // namespace dom
} // namespace iola
