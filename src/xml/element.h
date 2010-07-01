/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_XML_ELEMENT_H
#define IOLA_XML_ELEMENT_H

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

// STD
#include <map>
#include <vector>

// IOLA
#include "ielement.h"

namespace iola
{
namespace xml
{

////////////////////////////////////////////////////////////////////////////
// class iola::xml::element

class element :
	public ielement
{
public:
	element(const std::string strName);
	virtual void child(ielement* pkElement);
	virtual void attribute(std::string strKey, std::string strValue);
	virtual void text(std::string strText);
	virtual void xml(std::ostream& osXML);

protected:
	const std::string m_strName;

	typedef std::vector<ielement*> children_t;
	typedef children_t::iterator child_iterator_t;
	children_t m_vpkChildren;

	typedef std::map<std::string, std::string> attributes_t;
	typedef attributes_t::iterator attribute_iterator_t;
	attributes_t m_mapAttributes;
};

} // namespace xml
} // namespace iola

#endif // IOLA_XML_ELEMENT_H
