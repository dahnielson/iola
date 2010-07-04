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

// STD
#include <stdexcept>
#include <stack>
#include <vector>

// EXPAT
#include <expat.h>

// IOLA
#include <iola/string_cast.h>
#include "parser.h"

namespace
{

////////////////////////////////////////////////////////////////////////////
// class expat_parser

class expat_parser
{
public:
	expat_parser() :
		m_kParser(XML_ParserCreate(0))
	{
		XML_SetUserData(m_kParser, this);
		XML_SetStartElementHandler(m_kParser, rawStartElement);
		XML_SetEndElementHandler(m_kParser, rawEndElement);
		XML_SetCharacterDataHandler(m_kParser, rawCharacterData);
	}

	~expat_parser()
	{
		XML_ParserFree(m_kParser);
	}

	void parse(iola::xml::ifactory* pkFactory, iola::xml::ielement* pkRoot, std::istream& isXML)
	{
		m_pkFactory = pkFactory;

		while(!m_pkContext.empty())
			m_pkContext.pop();

		m_pkContext.push(pkRoot);

		std::vector<char> buffer(1024);
		for(isXML.read(&buffer[0], buffer.size()); isXML; isXML.read(&buffer[0], buffer.size()))
		{
			if(XML_STATUS_ERROR == XML_Parse(m_kParser, &buffer[0], isXML.gcount(), false))
				throw std::runtime_error(error_description());
		}
		if(XML_STATUS_ERROR == XML_Parse(m_kParser, &buffer[0], isXML.gcount(), true))
			throw std::runtime_error(error_description());
	}

private:
	const char* error_description()
	{
		static std::string buffer;
		buffer = XML_ErrorString(XML_GetErrorCode(m_kParser)) + std::string(" line: ") +
			iola::string_cast(XML_GetCurrentLineNumber(m_kParser)) + std::string(" column: ") +
			iola::string_cast(XML_GetCurrentColumnNumber(m_kParser));
		return buffer.c_str();
	}

	void startElement(const XML_Char* name, const XML_Char** attrs)
	{
		iola::xml::ielement* pkElement = m_pkFactory->factory(name);

		if (!pkElement)
			return;

		for(unsigned long i = 0; attrs[i]; i += 2)
			pkElement->attribute(attrs[i], attrs[i + 1]);

		if (!m_pkContext.empty())
			m_pkContext.top()->child(pkElement);

		m_pkContext.push(pkElement);
	}

	void endElement(const XML_Char* name)
	{
		if (m_pkContext.empty())
			throw std::runtime_error("No root element!");
		else
			m_pkContext.pop();
	}

	void characterData(const XML_Char* data, int length)
	{
		std::string strText(data, length);

		if (!m_pkContext.empty())
			m_pkContext.top()->text(strText);
	}

	static void rawStartElement(void* userData, const XML_Char* name, const XML_Char** attrs)
	{
		reinterpret_cast<expat_parser*>(userData)->startElement(name, attrs);
	}

	static void rawEndElement(void* userData, const XML_Char* name)
	{
		reinterpret_cast<expat_parser*>(userData)->endElement(name);
	}

	static void rawCharacterData(void* userData, const XML_Char* data, int length)
	{
		reinterpret_cast<expat_parser*>(userData)->characterData(data, length);
	}

	XML_Parser m_kParser;
	iola::xml::ifactory* m_pkFactory;
	std::stack<iola::xml::ielement*> m_pkContext;
};

} // namespace

namespace iola
{
namespace xml
{

////////////////////////////////////////////////////////////////////////////
// function iola::xml::parser

void parser(ifactory* pkFactory, ielement* pkRoot, std::istream& isXML)
{
	expat_parser* pkParser = new expat_parser();
	pkParser->parse(pkFactory, pkRoot, isXML);
}

} // namespace xml
} // namespace iola
