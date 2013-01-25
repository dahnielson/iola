/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IPLAYABLE_H
#define IOLA_MODEL_IPLAYABLE_H

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
#include <boost/signals2.hpp>

// IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// class iola::model::iplayable

/// Abstract interface of an object that can be played back
class iplayable :
	public virtual iola::iunknown
{
public:
	/// Signal emitted when playback changes
	boost::signals2::signal<void ()> on_playback_change_signal;

	/// Goto specific frame
	virtual void goto_frame(const int frame) = 0;
	/// Goto first frame
	virtual void goto_start() = 0;
	/// Goto last frame
	virtual void goto_end() = 0;
	/// Play forward, call repeatedly to alter playback speed
	virtual void play_forward() = 0;
	/// Play backward, call repeatedly to alter playback speed
	virtual void play_backward() = 0;
	/// Step forward one frame
	virtual void step_forward() = 0;
	/// Step backward one frame
	virtual void step_backward() = 0;
	/// Stop playback
	virtual void stop() = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IPLAYABLE_H
