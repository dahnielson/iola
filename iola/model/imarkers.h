/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IMARKERS_H
#define IOLA_MODEL_IMARKERS_H

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

class imarker_characteristics;
class iplayhead;

////////////////////////////////////////////////////////////////////////////
// class iola::model::imarkers

/// Abstract interface
class imarkers :
	virtual public iola::iunknown
{
public:
	/// Signal emitted when an in or out point changes
	boost::signals2::signal<void (const int, const int, const int, const int)> on_mark_change_signal;

	/// Set points on the object passed as parameter
	virtual void update_markers_on(imarker_characteristics* object) = 0;
	/// Set in point on the object passed as parameter
	virtual void set_in_point_on(imarkers* object) = 0;
	/// Set out point on the object passed as parameter
	virtual void set_out_point_on(imarkers* object) = 0;
	/// Set start point on the object passed as parameter
	virtual void set_start_point_on(imarkers* object) = 0;
	/// Set end point on the object passed as parameter
	virtual void set_end_point_on(imarkers* object) = 0;
	/// Set in point
	virtual void set_in_point(const int frame) = 0;
	/// Set out point
	virtual void set_out_point(const int frame) = 0;
	/// Set start point
	virtual void set_start_point(const int frame) = 0;
	/// Set end point
	virtual void set_end_point(const int frame) = 0;
	/// Goto in point
	virtual void goto_in_point_on(iplayhead* object) = 0;
	/// Goto out point
	virtual void goto_out_point_on(iplayhead* object) = 0;
	/// Goto start point
	virtual void goto_start_point_on(iplayhead* object) = 0;
	/// Goto end point
	virtual void goto_end_point_on(iplayhead* object) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IMARKERS_H
