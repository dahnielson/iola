/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IASSET_H
#define IOLA_MODEL_IASSET_H

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
#include <string>

// BOOST
#include <boost/signals2.hpp>

// MLT
#include <mlt++/MltConsumer.h>

//IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

class iaudio_settings;
class imarkers;
class iplayhead;
class ivideo_settings;

////////////////////////////////////////////////////////////////////////////
// class iola::model::iasset

class iasset :
	public virtual iola::iunknown
{
public:
	/// Signal emitted when producer changes with new start/end frame as parameter
	boost::signals2::signal<void (const int, const int)> on_producer_change_signal;

	/// Connect consumer
	virtual void connect_to(Mlt::Consumer* consumer) = 0;
	/// Set name
	virtual void set_name(const std::string name) = 0;
	/// Set uuid
	virtual void set_uuid(const std::string uuid) = 0;
	/// Set duration
	virtual void set_duration(const int duration) = 0;
	/// Set start timecode
	virtual void set_timecode(const std::string timecode) = 0;
	/// Return markers
	virtual imarkers* markers() = 0;
	/// Return video settings
	virtual ivideo_settings* video_settings() = 0;
	/// Return audio settings
	virtual iaudio_settings* audio_settings() = 0;
	/// Return playhead
	virtual iplayhead* playhead() = 0;
	/// Set start frame on markers
	virtual void set_start_on(imarkers* markers) = 0;
	/// Set end frame on markers
	virtual void set_end_on(imarkers* markers) = 0;
	/// Move playhead to start frame
	virtual void goto_start_on(iplayhead* playhead) = 0;
	/// Move playhead to end frame
	virtual void goto_end_on(iplayhead* playhead) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IASSET_H
