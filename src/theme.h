#ifndef IOLA_THEME_H
#define IOLA_THEME_H

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

#include "fltk.h"

namespace 
{

void melies_thin_up_box(int x, int y, int w, int h, Fl_Color c)
{
	int b = 1;

	// Fill
	fl_color(c);
	fl_rectf(x, y, w, h);

	// Hilite
	fl_color(FL_LIGHT2);
	fl_rectf(x, y, w, b);  // horizontal
	fl_rectf(x, y, b, h);  // vertical

	// Shadow
	fl_color(FL_DARK2);
	fl_rectf(x,     y+h-b, w, b);  // horizontal
	fl_rectf(x+w-b, y,     b, h);  // vertical
}

void melies_thin_down_box(int x, int y, int w, int h, Fl_Color c)
{
	int b = 1;

	// Fill
	fl_color(c);
	fl_rectf(x, y, w, h);

	// Hilite
	fl_color(FL_DARK2);
	fl_rectf(x, y, w, b);  // horizontal
	fl_rectf(x, y, b, h);  // vertical

	// Shadow
	fl_color(FL_LIGHT2);
	fl_rectf(x,     y+h-b, w, b);  // horizontal
	fl_rectf(x+w-b, y,     b, h);  // vertical
}

void melies_no_box(int x, int y, int w, int h, Fl_Color c)
{
}

} // namespace

namespace melies
{

void theme()
{
	// Set basic colors
	Fl::set_color(FL_BLACK, 19, 19, 19);
	Fl::set_color(FL_BLUE, 79, 79, 105);
	Fl::set_color(FL_GREEN, 89, 115, 115);
	Fl::set_color(FL_RED, 255, 62, 62);
	Fl::set_color(FL_WHITE, 255, 255, 255);
	Fl::set_color(FL_YELLOW, 196, 196, 117);


	// These are used as default colors in widgets and altered as necessary
	Fl::set_color(FL_FOREGROUND_COLOR, 196, 196, 196);
	Fl::set_color(FL_BACKGROUND2_COLOR, 107, 107, 107);
	Fl::set_color(FL_INACTIVE_COLOR, 107, 107, 107); 
	Fl::set_color(FL_SELECTION_COLOR, 107, 107, 107);

	// Boxtypes generally limit themselves to these colors
	Fl::set_color(FL_GRAY0, 77, 77, 77);
	Fl::set_color(FL_DARK3, 38, 38, 38);
	Fl::set_color(FL_DARK2, 55, 55, 55);
	Fl::set_color(FL_DARK1, 64, 64, 64);
	Fl::set_color(FL_BACKGROUND_COLOR, 77, 77, 77);
	Fl::set_color(FL_LIGHT1, 107, 107, 107);
	Fl::set_color(FL_LIGHT2, 138, 138, 138);
	Fl::set_color(FL_LIGHT3, 153, 153, 153);

	// Set box types
	Fl::set_boxtype(FL_THIN_UP_BOX, melies_thin_up_box, 1, 1, 2, 2);
	Fl::set_boxtype(FL_THIN_DOWN_BOX, melies_thin_down_box, 1, 1, 2, 2);
	Fl::set_boxtype(FL_UP_BOX, melies_thin_up_box, 1, 1, 2, 2);
	Fl::set_boxtype(FL_DOWN_BOX, melies_thin_down_box, 1, 1, 2, 2);

	// Boxtypes we don't care about ...
	//Fl::set_boxtype(FL_BORDER_BOX, melies_no_box, 1, 1, 2, 2);
	Fl::set_boxtype(FL_DOWN_FRAME, melies_no_box, 1, 1, 2, 2);
}

} // namespace melies

#endif // IOLA_THEME_H
