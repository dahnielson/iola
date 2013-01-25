/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_ISEQUENCE_WRITER_H
#define IOLA_MODEL_ISEQUENCE_WRITER_H

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

// IOLA
#include <iola/icommand.h>
#include <iola/model/iaudio_settings.h>
#include <iola/model/ivideo_settings.h>

namespace iola
{
namespace model
{

class iclip;

////////////////////////////////////////////////////////////////////////////
// class iola::model::isequence_writer

/// Abstract interface
class isequence_writer :
	public iola::icommand,
	public ivideo_settings,
	public iaudio_settings
{
public:
	/// Set name
	virtual void set_name(const std::string name) = 0;
	/// Set uuid
	virtual void set_uuid(const std::string uuid) = 0;
	/// Set duration
	virtual void set_duration(const int duration) = 0;
	/// Set timecode
	virtual void set_timecode(const std::string timecode) = 0;
	/// Append clipitem
	virtual void append_clipitem(iclip* clipitem) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_ISEQUENCE_WRITER_H
