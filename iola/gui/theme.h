/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

#ifndef IOLA_GUI_THEME_H
#define IOLA_GUI_THEME_H

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

void iola_thin_up_box(int x, int y, int w, int h, Fl_Color c)
{
	const int b = 1;

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

void iola_thin_down_box(int x, int y, int w, int h, Fl_Color c)
{
	const int b = 1;

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

void iola_no_box(int x, int y, int w, int h, Fl_Color c)
{
}

} // namespace

namespace iola
{
namespace gui
{
namespace rgb
{

	// Black
	inline void black()          { fl_color(  0,   0,   0); } // #000000 HSV:   0,   0,   0

	// Red -- http://colorschemedesigner.com/#0011Tw0w0w0w0
	inline void red_dark2()      { fl_color(166,   0,   0); } // #A60000
	inline void red_dark1()      { fl_color(191,  48,  48); } // #BF3030
	inline void red()            { fl_color(255,   0,   0); } // #FF0000 HSV:   0, 255, 255
	inline void red_light1()     { fl_color(255,  64,  64); } // #FF4040
	inline void red_light2()     { fl_color(255, 115, 115); } // #FF7373

	// Yellow -- http://colorschemedesigner.com/#1T11Tw0w0w0w0
	inline void yellow_dark2()   { fl_color(166, 166,   0); } // #A6A600
	inline void yellow_dark1()   { fl_color(191, 191,  48); } // #BFBF30
	inline void yellow()         { fl_color(255, 255,   0); } // #FFFF00 HSV:  43, 255, 255
	inline void yellow_light1()  { fl_color(255, 255,  64); } // #FFFF40
	inline void yellow_light2()  { fl_color(255, 255, 115); } // #FFFF73

	// Green -- http://colorschemedesigner.com/#2P11Tw0--w0w0
	inline void green_dark2()    { fl_color(  0, 166,   0); } // #00A600
	inline void green_dark1()    { fl_color( 48, 191,  48); } // #30BF30
	inline void green()          { fl_color(  0, 255,   0); } // #00FF00 HSV:  85, 255, 255
	inline void green_light1()   { fl_color( 64, 255,  64); } // #40FF40
	inline void green_light2()   { fl_color(115, 255, 115); } // #73FF73

	// Cyan -- http://colorschemedesigner.com/#3i11Tw0--w0w0
	inline void cyan_dark2()     { fl_color(  0, 166, 166); } // #00A6A6
	inline void cyan_dark1()     { fl_color( 48, 191, 191); } // #30BFBF
	inline void cyan()           { fl_color(  0, 255, 255); } // #00FFFF HSV: 128, 255, 255
	inline void cyan_light1()    { fl_color( 64, 255, 255); } // #40FFFF
	inline void cyan_light2()    { fl_color(115, 255, 255); } // #73FFFF

	// Blue -- http://colorschemedesigner.com/#3-11T----w0w0
	inline void blue_dark2()     { fl_color(  0,   0, 166); } // #0000A6
	inline void blue_dark1()     { fl_color( 48,  48, 191); } // #3030BF
	inline void blue()           { fl_color(  0,   0, 255); } // #0000FF HSV: 170, 255, 255
	inline void blue_light1()    { fl_color( 64,  64, 255); } // #4040FF
	inline void blue_light2()    { fl_color(115, 115, 255); } // #7373FF

	// Magenta -- http://colorschemedesigner.com/#4W11Tw0--w0w0
	inline void magenta_dark2()  { fl_color(166,   0, 166); } // #A600A6
	inline void magenta_dark1()  { fl_color(191,  48, 191); } // #BF30BF
	inline void magenta()        { fl_color(255,   0, 255); } // #FF00FF HSV: 213, 255, 255
	inline void magenta_light1() { fl_color(255,  64, 255); } // #FF40FF
	inline void magenta_light2() { fl_color(255, 115, 255); } // #FF73FF

	// White
	inline void white()          { fl_color(255, 255, 255); } // #FFFFFF

	// Theme
	void theme()
	{
		// Set basic color scheme
		Fl::set_color(FL_BLACK,               0,   0,   0); // #000000
		Fl::set_color(FL_RED,               255,   0,   0); // #FF0000
		Fl::set_color(FL_GREEN,               0, 255,   0); // #00FF00
		Fl::set_color(FL_BLUE,                0,   0, 255); // #0000FF
		Fl::set_color(FL_CYAN,                0, 255, 255); // #00FFFF
		Fl::set_color(FL_MAGENTA,           255,   0, 255); // #FF00FF
		Fl::set_color(FL_YELLOW,            255, 255,   0); // #FFFF00
		Fl::set_color(FL_DARK_RED,          191,  48,  48); // #BF3030
		Fl::set_color(FL_DARK_GREEN,         48, 191,  48); // #30BF30
		Fl::set_color(FL_DARK_BLUE,          48,  48, 191); // #3030BF
		Fl::set_color(FL_DARK_CYAN,          48, 191, 191); // #30BFBF
		Fl::set_color(FL_DARK_MAGENTA,      191,  48, 191); // #BF30BF
		Fl::set_color(FL_DARK_YELLOW,       191, 191,  48); // #BFBF30
		Fl::set_color(FL_WHITE,             255, 255, 255); // #FFFFFF

		// These are used as default colors in widgets
		Fl::set_color(FL_FOREGROUND_COLOR,  196, 196, 196);
		Fl::set_color(FL_BACKGROUND2_COLOR, 107, 107, 107);
		Fl::set_color(FL_INACTIVE_COLOR,    107, 107, 107);
		Fl::set_color(FL_SELECTION_COLOR,   107, 107, 107);

		// Boxtypes generally limit themselves to these colors
		Fl::set_color(FL_GRAY0,              77,  77,  77);
		Fl::set_color(FL_DARK3,              38,  38,  38);
		Fl::set_color(FL_DARK2,              55,  55,  55);
		Fl::set_color(FL_DARK1,              64,  64,  64);
		Fl::set_color(FL_BACKGROUND_COLOR,   77,  77,  77); // #4D4D4D
		Fl::set_color(FL_LIGHT1,            107, 107, 107);
		Fl::set_color(FL_LIGHT2,            138, 138, 138);
		Fl::set_color(FL_LIGHT3,            153, 153, 153);

		// Set box types
		Fl::set_boxtype(FL_THIN_UP_BOX,   iola_thin_up_box,   1, 1, 2, 2);
		Fl::set_boxtype(FL_THIN_DOWN_BOX, iola_thin_down_box, 1, 1, 2, 2);
		Fl::set_boxtype(FL_UP_BOX,        iola_thin_up_box,   1, 1, 2, 2);
		Fl::set_boxtype(FL_DOWN_BOX,      iola_thin_down_box, 1, 1, 2, 2);
	}

} // namespace rgb

namespace scheme
{

	// Black
	inline void black()          { fl_color( 19,  19,  19); } // #131313 HSV:   0,   0,  19

	// Red -- http://colorschemedesigner.com/#0011To6w0w0w0
	inline void red_dark2()      { fl_color(166,  20,  20); } // #A61414
	inline void red_dark1()      { fl_color(191,  83,  83); } // #BF5353
	inline void red()            { fl_color(255,  63,  63); } // #FF3F3F HSV:   0, 192, 255
	inline void red_light1()     { fl_color(255, 111, 111); } // #FF6F6F
	inline void red_light2()     { fl_color(255, 149, 149); } // #FF9595

	// Yellow -- http://colorschemedesigner.com/#1V11To6--w0w0
	inline void yellow_dark2()   { fl_color(162, 166,  20); } // #A2A614
	inline void yellow_dark1()   { fl_color(188, 191,  83); } // #BCBF53
	inline void yellow()         { fl_color(250, 255,  63); } // #FAFF3F HSV:  44, 192, 255
	inline void yellow_light1()  { fl_color(251, 255, 111); } // #FBFF6F
	inline void yellow_light2()  { fl_color(252, 255, 149); } // #FCFF95

	// Green -- http://colorschemedesigner.com/#2P11T89hTw0w0
	inline void green_dark2()    { fl_color( 28,  74,  28); } // #1C4A1C
	inline void green_dark1()    { fl_color( 69,  86,  69); } // #455645
	inline void green()          { fl_color( 85, 114,  85); } // #557255 HSV:  85,  65, 114
	inline void green_light1()   { fl_color(149, 185, 149); } // #95B995
	inline void greem_light2()   { fl_color(159, 185, 159); } // #9FB99F

	// Cyan -- http://colorschemedesigner.com/#3i11T89nRw0w0
	inline void cyan_dark2()     { fl_color( 28,  74,  74); } // #1C4A4A
	inline void cyan_dark1()     { fl_color( 69,  86,  86); } // #455656
	inline void cyan()           { fl_color( 85, 114, 114); } // #557272 HSV: 128,  65, 114
	inline void cyan_light1()    { fl_color(149, 185, 185); } // #95B9B9
	inline void cyan_light2()    { fl_color(159, 185, 185); } // #9FB9B9

	// Blue -- http://colorschemedesigner.com/#3-11T9Aksw0w0
	inline void blue_dark2()     { fl_color( 28,  28,  74); } // #1C1C4A
	inline void blue_dark1()     { fl_color( 69,  69,  86); } // #454556
	inline void blue()           { fl_color( 85,  85, 114); } // #555572 HSV: 170,  65, 114
	inline void blue_light1()    { fl_color(149, 149, 185); } // #9595B9
	inline void blue_light2()    { fl_color(159, 159, 185); } // #9F9FB9

	// Magenta -- http://colorschemedesigner.com/#4W11T89m0w0w0
	inline void magenta_dark2()  { fl_color( 74,  28,  74); } // #4A1C4A
	inline void magenta_dark1()  { fl_color( 86,  69,  86); } // #564556
	inline void magenta()        { fl_color(114,  85, 114); } // #725572 HSV: 213,  65, 114
	inline void magenta_light1() { fl_color(185, 149, 185); } // #B995B9
	inline void magenta_light2() { fl_color(185, 159, 185); } // #B99FB9

	// White
	inline void white()          { fl_color(255, 255, 255); } // #FFFFFF HSV:   0,   0, 255

	// Theme
	void theme()
	{
		// Set basic color scheme
		Fl::set_color(FL_BLACK,              19,  19,  19); // #131313
		Fl::set_color(FL_RED,               255,  63,  63); // #FF3F3F
		Fl::set_color(FL_GREEN,              85, 114,  85); // #557255
		Fl::set_color(FL_BLUE,               85,  85, 114); // #555572
		Fl::set_color(FL_CYAN,               85, 114, 114); // #557272
		Fl::set_color(FL_MAGENTA,           114,  85, 114); // #725572
		Fl::set_color(FL_YELLOW,            250, 255,  63); // #FAFF3F
		Fl::set_color(FL_DARK_RED,          191,  83,  83); // #BF5353
		Fl::set_color(FL_DARK_GREEN,         69,  86,  69); // #455645
		Fl::set_color(FL_DARK_BLUE,          69,  69,  86); // #454556
		Fl::set_color(FL_DARK_CYAN,          69,  86,  86); // #455656
		Fl::set_color(FL_DARK_MAGENTA,       86,  69,  86); // #564556
		Fl::set_color(FL_DARK_YELLOW,       188, 191,  83); // #BCBF53
		Fl::set_color(FL_WHITE,             255, 255, 255); // #FFFFFF

		// These are used as default colors in widgets
		Fl::set_color(FL_FOREGROUND_COLOR,  196, 196, 196);
		Fl::set_color(FL_BACKGROUND2_COLOR, 107, 107, 107);
		Fl::set_color(FL_INACTIVE_COLOR,    107, 107, 107);
		Fl::set_color(FL_SELECTION_COLOR,   107, 107, 107);

		// Boxtypes generally limit themselves to these colors
		Fl::set_color(FL_GRAY0,              77,  77,  77);
		Fl::set_color(FL_DARK3,              38,  38,  38);
		Fl::set_color(FL_DARK2,              55,  55,  55);
		Fl::set_color(FL_DARK1,              64,  64,  64);
		Fl::set_color(FL_BACKGROUND_COLOR,   77,  77,  77); // #4D4D4D
		Fl::set_color(FL_LIGHT1,            107, 107, 107);
		Fl::set_color(FL_LIGHT2,            138, 138, 138);
		Fl::set_color(FL_LIGHT3,            153, 153, 153);

		// Set box types
		Fl::set_boxtype(FL_THIN_UP_BOX,   iola_thin_up_box,   1, 1, 2, 2);
		Fl::set_boxtype(FL_THIN_DOWN_BOX, iola_thin_down_box, 1, 1, 2, 2);
		Fl::set_boxtype(FL_UP_BOX,        iola_thin_up_box,   1, 1, 2, 2);
		Fl::set_boxtype(FL_DOWN_BOX,      iola_thin_down_box, 1, 1, 2, 2);
	}

} // namespace scheme
} // namespace gui
} // namespace iola

#endif // IOLA_GUI_THEME_H
