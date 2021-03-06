/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOM_FILE_ELEMENT_H
#define IOLA_DOM_FILE_ELEMENT_H

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

// BOOST
#include <boost/filesystem.hpp>

// IOLA
#include <iola/model/iclip.h>
#include <iola/xml/ielement.h>
#include <iola/xml/ivisitor.h>

namespace  iola
{
namespace dom
{

class duration_element;
class name_element;
class pathurl_element;
class rate_element;

////////////////////////////////////////////////////////////////////////////
// class iola::dom::file_element

class file_element :
	public iola::xml::ielement
{
public:
	file_element(const std::string strName);
	~file_element();
	void child(iola::xml::ielement* pkElement);
	void attribute(std::string strKey, std::string strValue);
	void text(std::string strText);
	void xml(std::ostream& osXML);
	void accept(iola::xml::ivisitor* visitor);

	void restore(iola::model::iclip* object);

private:
	const std::string m_strName; 
	std::string m_strID;
	duration_element* m_pkDuration;
	rate_element* m_pkRate;
	name_element* m_pkName;
	pathurl_element* m_pkPathURL;
};

} // namespace dom
} // namespace iola

#endif // IOLA_DOM_FILE_ELEMENT_H
