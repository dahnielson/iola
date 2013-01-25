/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_ICONSUMABLE_H
#define IOLA_MODEL_ICONSUMABLE_H

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
#include <boost/rational.hpp>
#include <boost/signals2.hpp>

// MLT
#include <mlt++/MltConsumer.h>

// IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// class iola::model::iconsumable

/// Abstract interface of an object that can be consumed
class iconsumable :
	public virtual iola::iunknown
{
public:
	/// Signal emitted when producer changes with new start/end frame as parameter
	boost::signals2::signal<void (const int, const int)> on_producer_change_signal;
	/// Signal emitted when the width changes
	boost::signals2::signal<void (const int)> on_width_change_signal;
	/// Signal emitted when the height changes
	boost::signals2::signal<void (const int)> on_height_change_signal;
	/// Signal emitted when the Display Aspect Ratio changes
	boost::signals2::signal<void (const boost::rational<int>)> on_dar_change_signal;
	/// Signal emitted when the Pixel Aspect Ratio changes
	boost::signals2::signal<void (const boost::rational<int>)> on_par_change_signal;
	/// Signal emitted when the progressive flag changes
	boost::signals2::signal<void (const bool)> on_progressive_change_signal;
	/// Signal emitted when the frame rate changes
	boost::signals2::signal<void (const boost::rational<int>)> on_fps_change_signal;
	/// Signal emitted when the timebase changes
	boost::signals2::signal<void (const int)> on_timebase_change_signal;
	/// Signal emitted when the ntsc flag changes
	boost::signals2::signal<void (const bool)> on_ntsc_change_signal;
	/// Signal emitted when the audio sample depth changes
	boost::signals2::signal<void (const int)> on_sample_depth_change_signal;
	/// Signal emitted when the audio sample rate changes
	boost::signals2::signal<void (const int)> on_sample_rate_change_signal;

	/// Connect consumer
	virtual void connect_to(Mlt::Consumer* consumer) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_ICONSUMABLE_H
