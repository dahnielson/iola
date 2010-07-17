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

#include "bool_terminal.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::bool_terminal

bool_terminal::bool_terminal(const std::string strName) :
        m_strName(strName)
{}

void
bool_terminal::child(iola::xml::ielement* pkElement)
{
	// We're terminal. And we've had a vasectomy!
}

void
bool_terminal::attribute(std::string strKey, std::string strValue)
{
	// No attributes.
}

void
bool_terminal::text(std::string strText)
{
	if (strText == "TRUE" || strText == "true" || strText == "True")
		m_bValue = true;
	else if (strText == "FALSE" || strText == "false" || strText == "False")
		m_bValue = false;
}

void
bool_terminal::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName << ">";
	if (m_bValue)
		osXML << "TRUE";
	else
		osXML << "FALSE";
	osXML << "</" << m_strName << ">" << std::endl;
}

bool
bool_terminal::get()
{
	return m_bValue;
}

void
bool_terminal::set(bool bValue)
{
	m_bValue = bValue;
}

} // namespace dom
} // namespace iola
