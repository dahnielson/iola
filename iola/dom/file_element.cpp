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

#include "duration_element.h"
#include "file_element.h"
#include "name_element.h"
#include "pathurl_element.h"
#include "rate_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::file_element

file_element::file_element(const std::string strName) :
        m_strName(strName),
	m_pkDuration(0),
	m_pkRate(0),
	m_pkName(0),
	m_pkPathURL(0)
{}

file_element::~file_element()
{
	if (m_pkDuration)
		delete m_pkDuration;
	if (m_pkRate)
		delete m_pkRate;
	if (m_pkName)
		delete m_pkName;
	if (m_pkPathURL)
		delete m_pkPathURL;
}

void
file_element::child(iola::xml::ielement* pkElement)
{
	if (dynamic_cast<duration_element*>(pkElement))
		m_pkDuration = dynamic_cast<duration_element*>(pkElement);
	else if (dynamic_cast<rate_element*>(pkElement))
		m_pkRate = dynamic_cast<rate_element*>(pkElement);
	else if (dynamic_cast<name_element*>(pkElement))
		m_pkName = dynamic_cast<name_element*>(pkElement);
	else if (dynamic_cast<pathurl_element*>(pkElement))
		m_pkPathURL = dynamic_cast<pathurl_element*>(pkElement);
}

void
file_element::attribute(std::string strKey, std::string strValue)
{
	if (strKey == "id")
		m_strID = strValue;
}

void
file_element::text(std::string strText)
{
}

void
file_element::xml(std::ostream& osXML)
{
	if (m_strID.empty())
		osXML << "<" << m_strName << ">" << std::endl;
	else
		osXML << "<" << m_strName << " id=\"" << m_strID << "\">" << std::endl;
	if (m_pkDuration)
		m_pkDuration->xml(osXML);
	if (m_pkRate)
		m_pkRate->xml(osXML);
	if (m_pkName)
		m_pkName->xml(osXML);
	if (m_pkPathURL)
		m_pkPathURL->xml(osXML);
	osXML << "</" << m_strName << ">" << std::endl;
}

void
file_element::accept(iola::xml::ivisitor* visitor)
{
	visitor->visit(this);
	if (m_pkDuration)
		m_pkDuration->accept(visitor);
	if (m_pkRate)
		m_pkRate->accept(visitor);
	if (m_pkName)
		m_pkName->accept(visitor);
	if (m_pkPathURL)
		m_pkPathURL->accept(visitor);
}

void
file_element::restore(iola::model::iclip* object)
{
	if (m_pkDuration)
		m_pkDuration->restore(object);
	if (m_pkRate)
		m_pkRate->restore(object->video_settings());
	if (m_pkName)
		m_pkName->restore(object);
	if (m_pkPathURL)
		m_pkPathURL->restore(object->file());
}

} // namespace dom
} // namespace iola
