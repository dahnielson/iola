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

// RLOG
#include <rlog/rlog.h>

// IOLA
#include "dummy.h"

namespace
{

////////////////////////////////////////////////////////////////////////////
// class cdata

class cdata :
	public iola::xml::ielement
{
public:
	cdata(const std::string strText) :
		m_strText(strText)
	{}

	void child(iola::xml::ielement*)
	{}

	void attribute(std::string, std::string)
	{}

	void text(std::string)
	{}

	void xml(std::ostream& osXML)
	{
		osXML << m_strText;
	}

private:
	const std::string m_strText;
};

} // namespace

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::dummy

dummy::dummy(const std::string strName) :
        m_strName(strName)
{
	rWarning("%s: Stand in dummy is used for element %s", __PRETTY_FUNCTION__, m_strName.c_str());
}

void
dummy::child(iola::xml::ielement* pkElement)
{
	m_vpkChildren.push_back(pkElement);
}

void
dummy::attribute(std::string strKey, std::string strValue)
{
	m_mapAttributes[strKey] = strValue;
}

void
dummy::text(std::string strText)
{
	m_vpkChildren.push_back( new cdata(strText) );
}

void
dummy::xml(std::ostream& osXML)
{
	osXML << "<" << m_strName;

	for (attribute_iterator_t i = m_mapAttributes.begin();
	     i != m_mapAttributes.end(); i++)
		osXML << " " << (*i).first << "=\"" << (*i).second << "\"";

	osXML << ">" << std::endl;

	for (child_iterator_t i = m_vpkChildren.begin();
	     i != m_vpkChildren.end(); i++)
		(*i)->xml(osXML);

	osXML << "</" << m_strName << ">" << std::endl;;
}

void
dummy::restore()
{
	rWarning("%s: Stand in dummy for element %s can't call restore()", __PRETTY_FUNCTION__, m_strName.c_str());
}

void
dummy::store()
{
	rWarning("%s: Stand in dummy for element %s can't call store()", __PRETTY_FUNCTION__, m_strName.c_str());
}

} // namespace dom
} // namespace iola
