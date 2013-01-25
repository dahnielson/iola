/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IPROGRAM_H
#define IOLA_MODEL_IPROGRAM_H

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
#include "iconsumable.h"
#include "iloadable.h"
#include "imarkable.h"
#include "iplayable.h"
#include "isavable.h"

namespace iola
{
namespace model
{

class iclip;
class isequence;

////////////////////////////////////////////////////////////////////////////
// class iola::model::iprogram

/// Abstract interface of a domain model program object
class iprogram :
	public iloadable,
	public isavable,
	public iconsumable,
	public iplayable,
	public imarkable
{
public:
	/// Signal emitted on alert
	boost::signals2::signal<void (std::string)> on_alert_signal;

	/// Clear program
	virtual void clear() = 0;
	/// Load a sequence
	virtual void load_sequence(isequence* sequence) = 0;
	/// Returns sequence
	virtual isequence* sequence() = 0;
	/// Goto previous edit
	virtual void goto_previous_edit() = 0;
	/// Goto the next edit
	virtual void goto_next_edit() = 0;
	/// Lift marked material and leave a gap
	virtual void make_lift_edit() = 0;
	/// Extract marked material and do not leave a gap
	virtual void make_extract_edit() = 0;
	/// Insert material
	virtual void make_overwrite_edit(iola::model::iclip* clipitem) = 0;
	/// Overwrite material
	virtual void make_insert_edit(iola::model::iclip* clipitem) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IPROGRAM_H
