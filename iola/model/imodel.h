/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IMODEL_H
#define IOLA_MODEL_IMODEL_H

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

class isource;
class iprogram;

////////////////////////////////////////////////////////////////////////////
// class iola::model::imodel

/// Abstract interface
class imodel :
	virtual public iola::iunknown
{
public:
	/// Signal emitted on alert
	boost::signals2::signal<void (std::string)> on_alert_signal;

	/// Return the source
	virtual isource* source() = 0;
	/// Return the program
	virtual iprogram* program() = 0;
	/// Lift marked material and leave a gap
	virtual void make_lift_edit() = 0;
	/// Extract marked material and do not leave a gap
	virtual void make_extract_edit() = 0;
	/// Insert material
	virtual void make_insert_edit() = 0;
	/// Overwrite material
	virtual void make_overwrite_edit() = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IMODEL_H
