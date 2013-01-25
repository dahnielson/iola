/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IMARKABLE_H
#define IOLA_MODEL_IMARKABLE_H

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
// class iola::model::imarkable

/// Abstract interface of an object that can be marked
class imarkable :
	public virtual iola::iunknown
{
public:
	/// Signal emitted when an in or out point changes
	boost::signals2::signal<void (const int, const int, const int, const int)> on_mark_change_signal;

	/// Mark in point
	virtual void mark_in() = 0;
	/// Mark out point
	virtual void mark_out() = 0;
	/// Set in and out point to begining and end of clip
	virtual void mark_clip() = 0;
	/// Remove in point
	virtual void clear_mark_in() = 0;
	/// Remove out point
	virtual void clear_mark_out() = 0;
	/// Goto in point
	virtual void goto_mark_in() = 0;
	/// Goto out point
	virtual void goto_mark_out() = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IMARKABLE_H
