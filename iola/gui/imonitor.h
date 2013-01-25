/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_IMONITOR_H
#define IOLA_GUI_IMONITOR_H

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

// IOLA
#include <iola/model/iaudio_characteristics.h>
#include <iola/model/imodel.h>
#include <iola/model/ivideo_characteristics.h>

namespace iola
{
namespace gui
{

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::imonitor

/// Abstract interface of program monitor
class imonitor :
	public iola::model::ivideo_characteristics,
	public iola::model::iaudio_characteristics
{
public:
	/// Connect monitor to program
	virtual void connect_to(iola::model::imodel* model) = 0;
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_IMONITOR_H
