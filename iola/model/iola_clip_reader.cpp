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
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// RLOG
#include <rlog/rlog.h>

// IOLA
#include <iola/dom/element_factory.h>
#include <iola/dom/root.h>
#include <iola/model/iclip.h>
#include <iola/model/iclip_reader.h>
#include <iola/xml/parser.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::iola_clip_reader_implementation

class iola_clip_reader_implementation :
	public iola::model::iclip_reader
{
public:
	iola_clip_reader_implementation(const boost::filesystem::path file) :
		m_kFile(file)
	{
	}

	~iola_clip_reader_implementation()
	{
	}

	void read_file_into(iola::model::iclip* clip)
	{
		iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
		iola::dom::root* pkRoot = new iola::dom::root();
		boost::filesystem::ifstream isXML(m_kFile);
		try
		{
			iola::xml::parser(pkFactory, pkRoot, isXML);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			delete pkFactory;
			delete pkRoot;
			return;
		}
		pkRoot->restore(clip);
		delete pkFactory;
		delete pkRoot;
	}

private:
	const boost::filesystem::path m_kFile;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_iola_clip_reader

iclip_reader* create_iola_clip_reader(const boost::filesystem::path file)
{
	return new iola_clip_reader_implementation(file);
}

} // namespace model
} // namespace iola
