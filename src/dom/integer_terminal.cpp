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

// BOOS
#include <boost/lexical_cast.hpp>

// IOLA
#include "integer_terminal.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::integer_terminal

integer_terminal::integer_terminal(const std::string strName) :
        m_strName(strName)
{}

void
integer_terminal::child(iola::xml::ielement* pkElement)
{
	// We're terminal. And we've had a vasectomy!
}

void
integer_terminal::attribute(std::string strKey, std::string strValue)
{
	// No attributes.
}

void
integer_terminal::text(std::string strText)
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
integer_terminal::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	osXML << m_iValue;
	osXML << "</" << m_strName << ">";
}

int
integer_terminal::get()
{
	return m_iValue;
}

} // namespace dom
} // namespace iola
