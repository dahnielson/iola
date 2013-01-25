/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_ISEQUENCE_H
#define IOLA_MODEL_ISEQUENCE_H

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

// MLT
#include <mlt++/MltPlaylist.h>

// IOLA
#include "iasset.h"

namespace iola
{
namespace model
{

class iclip;
class ifile;
class imarkers;
class iplayhead;
class isequence_writer;

////////////////////////////////////////////////////////////////////////////
// class iola::model::isequence

/// Abstract interface
class isequence :
	public iasset
{
public:
	/// Write sequence to clip writer
	virtual void write_sequence_to(isequence_writer* writer) = 0;
	/// Cut region from sequence using start/end points on markers
	virtual void cut_region(iola::model::imarkers* markers) = 0;
	/// Insert blank into the sequence using start/end points on markers
	virtual void insert_blank(iola::model::imarkers* markers) = 0;
	/// Insert clip into the sequence using in/out/start/end points on clip
	virtual void insert_clip(iola::model::iclip* clip) = 0;
	/// Goto previous edit
	virtual void goto_previous_edit_on(iplayhead* object) = 0;
	/// Goto next edit
	virtual void goto_next_edit_on(iplayhead* object) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_ISEQUENCE_H
