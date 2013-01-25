/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IPLAYHEAD_H
#define IOLA_MODEL_IPLAYHEAD_H

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
#include <boost/signals2.hpp>

// IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

class imarkers;

////////////////////////////////////////////////////////////////////////////
// class iola::model::iplayhead

class iplayhead :
	virtual public iola::iunknown
{
public:
	/// Signal emitted when playback changes
	boost::signals2::signal<void ()> on_playback_change_signal;

	/// Set current frame as in point on markers
	virtual void set_in_point_on(imarkers* markers) = 0;
	/// Set current frame as out point on markers
	virtual void set_out_point_on(imarkers* markers) = 0;
	/// Set current frame as start point on markers
	virtual void set_start_point_on(imarkers* markers) = 0;
	/// Set current frame as end point on markers
	virtual void set_end_point_on(imarkers* markers) = 0;
	/// Moves playhead to specific frame
	virtual void set_frame(const int frame) = 0;
	/// Moves playhead to relative frame
	virtual void set_relative_frame(const int frame) = 0;
	/// Set playhead speed
	virtual void set_speed(const double speed) = 0;
	/// Set relative playhead speed
	virtual void set_relative_speed(const double speed) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IPLAYHEAD_H
