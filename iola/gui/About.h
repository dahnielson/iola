/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_ABOUT_H
#define IOLA_GUI_ABOUT_H

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

// FLTK
#include "fltk.h"

namespace iola
{
namespace gui
{

/// About dialog
class About :
	public Fl_Double_Window
{
public:
	About();
	~About();

private:
	static void close_window(Fl_Widget*, void* v) { reinterpret_cast<About*>(v)->close_window(); }

	void close_window();
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_ABOUT_H
