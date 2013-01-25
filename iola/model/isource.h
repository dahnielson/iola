/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_ISOURCE_H
#define IOLA_MODEL_ISOURCE_H

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
#include "iconsumable.h"
#include "iloadable.h"
#include "imarkable.h"
#include "iplayable.h"

namespace iola
{
namespace model
{

class iclip;

////////////////////////////////////////////////////////////////////////////
// class iola::model::isource

/// Abstract interface of a domain model source object
class isource :
	public iloadable,
	public iconsumable,
	public iplayable,
	public imarkable
{
public:
	/// Signal emitted on alert
	boost::signals2::signal<void (std::string)> on_alert_signal;

	/// Clear source
	virtual void clear() = 0;
	/// Load a clip
	virtual void load_clip(iclip* clip) = 0;
	/// Returns clip
	virtual iclip* clip() = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_ISOURCE_H
