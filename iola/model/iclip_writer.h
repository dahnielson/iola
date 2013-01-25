/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_ICLIP_WRITER_H
#define IOLA_MODEL_ICLIP_WRITER_H

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

// IOLA
#include <iola/model/iaudio_settings.h>
#include <iola/model/ivideo_settings.h>

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// class iola::model::iclip_writer

/// Abstract interface
class iclip_writer :
	public ivideo_settings,
	public iaudio_settings
{
public:
	/// Set name to write
	virtual void set_name(const std::string name) = 0;
	/// Set duration to write
	virtual void set_duration(const int duration) = 0;
	/// Set timecode to write
	virtual void set_timecode(const std::string timecode) = 0;
	/// Set file to write
	virtual void set_file(const boost::filesystem::path file) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_ICLIP_WRITER_H
