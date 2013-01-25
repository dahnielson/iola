/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IAUDIO_SETTINGS_H
#define IOLA_MODEL_IAUDIO_SETTINGS_H

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

class iaudio_characteristics;
class iplayable;

////////////////////////////////////////////////////////////////////////////
// class iola::model::iaudio_settings

/// Abstract interface
class iaudio_settings :
	virtual public iola::iunknown
{
public:
	/// Signal emitted when the audio sample depth changes
	boost::signals2::signal<void (const int)> on_sample_depth_change_signal;
	/// Signal emitted when the audio sample rate changes
	boost::signals2::signal<void (const int)> on_sample_rate_change_signal;

	/// Update settings on the object passed as parameter
	virtual void update_settings_on(iaudio_settings* object) = 0;
	/// Update settings on the object passed as parameter
	virtual void update_settings_on(iaudio_characteristics* object) = 0;
	/// Set audio bit depth 
	virtual void set_sample_depth(const int depth) = 0;
	/// Set audio sample rate
	virtual void set_sample_rate(const int rate) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IAUDIO_SETTINGS_H
