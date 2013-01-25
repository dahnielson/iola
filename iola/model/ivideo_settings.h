/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IVIDEO_SETTINGS_H
#define IOLA_MODEL_IVIDEO_SETTINGS_H

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

// IOLA
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

class iplayable;
class ivideo_characteristics;

////////////////////////////////////////////////////////////////////////////
// class iola::model::ivideo_settings

/// Abstract interface
class ivideo_settings :
	virtual public iola::iunknown
{
public:
	/// Enumerates Pixel Aspect Ratios
	enum par_t {
		SQUARE,
		NTSC_601,
		PAL_601,
		HD_960x720,
		HD_1280x1080,
		HD_1440x1080
	};

	/// Enumerates field dominances
	enum field_t {
		NONE,
		EVEN,
		ODD
	};

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

	/// Update settings on the object passed as parameter
	virtual void update_settings_on(ivideo_settings* object) = 0;
	/// Update settings on the object passed as parameter
	virtual void update_settings_on(ivideo_characteristics* object) = 0;
	/// Set storage width of frame in pixels
	virtual void set_width(const int width) = 0;
	/// Set storage height of frame in pixels
	virtual void set_height(const int height) = 0;
	/// Set Pixel Aspect Ratio of frame
	virtual void set_par(const par_t par) = 0;
	/// Set if image is anamorphic (16:9)
	virtual void set_anamorphic(const bool anamorphic) = 0;
	/// Set field dominance
	virtual void set_field_dominance(const field_t field) = 0;
	/// Set frame rates timebase (e.g. 24, 25, 30, etc.)
	virtual void set_fps_timebase(const int timebase) = 0;
	/// Set if frame rate is NTSC
	virtual void set_fps_ntsc(const bool ntsc) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IVIDEO_SETTINGS_H
