/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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

// IOLA
#include "igui.h"

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::gui_implementation

class gui_implementation :
	public iola::gui::igui
{
public:
	gui_implementation()
	{
	}

	~gui_implementation()
	{
	}

	void connect_to(iola::model::imodel* model)
	{
	}

	void show()
	{
	}

	void stop()
	{
	}
};


} // namespace

namespace iola
{
namespace gui
{

////////////////////////////////////////////////////////////////////////////
// function iola::gui::create_gui

igui* create_gui()
{
	return new gui_implementation();
}

} // namespace gui
} // namespace iola
