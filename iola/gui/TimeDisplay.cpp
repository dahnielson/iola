/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

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
#include <FL/Fl.H>
#include <FL/fl_draw.H>

// IOLA
#include "TimeDisplay.h"

namespace iola
{
namespace gui
{

TimeDisplay::TimeDisplay(int x, int y, int w, int h, const char *label) :
	Fl_Widget(x, y, w, h),
	m_bDropFrame(true),
	m_kTimecode(30)
{
	box(FL_DOWN_BOX);
	labelcolor(FL_FOREGROUND_COLOR);
	labelfont(FL_SCREEN);
	labelsize(12);
}

void TimeDisplay::set_timebase(const int timebase)
{
	m_kTimecode.set_timebase(timebase);
	redraw();
}

void TimeDisplay::set_dropframe(const bool dropframe)
{
	m_bDropFrame = dropframe;
	redraw();
}

void TimeDisplay::value(const int v)
{
	m_kTimecode.set_framecount(v);
//	redraw(); //XXX this will cause a spectacular crash
}

void TimeDisplay::draw()
{
	// Calculate area inside box
	int X = x() + Fl::box_dx(box());
	int Y = y() + Fl::box_dy(box());
	int W = w() - Fl::box_dw(box());
	int H = h() - Fl::box_dh(box());

	// Draw background
	fl_push_clip(x(), y(), w(), h());
	draw_box();
	fl_pop_clip();

	// Draw timecode
	fl_color(labelcolor());
	fl_font(labelfont(), labelsize());
	const char* time = m_kTimecode.get_timecode(m_bDropFrame).c_str();
	fl_draw(time, X, Y, W, H, align());
}

} // namespace gui
} // namespace iola
