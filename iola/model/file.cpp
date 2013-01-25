/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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

// IOLA
#include <iola/model/ifile.h>
#include <iola/model/ifile_characteristics.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::file_implementation

class file_implementation :
	public iola::model::ifile
{
public:
	file_implementation()
	{
	}

	~file_implementation()
	{
	}

	void set_pathurl_on(iola::model::ifile_characteristics* file)
	{
		file->set_pathurl(m_kFile);
	}

	void set_pathurl_on(iola::model::ifile* file)
	{
		file->set_pathurl(m_kFile);
	}

	void set_pathurl(boost::filesystem::path file)
	{
		m_kFile = file;
	}

private:
	boost::filesystem::path m_kFile;
};


} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_file

ifile* create_file()
{
	return new file_implementation();
}

} // namespace model
} // namespace iola
