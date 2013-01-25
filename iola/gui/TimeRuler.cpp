/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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
#include "TimeRuler.h"

namespace iola
{
namespace gui
{

TimeRuler::TimeRuler(int x, int y, int w, int h, const char *label) :
	Fl_Valuator(x, y, w, h, label),
	m_dIn(-1),
	m_dOut(-1)
{
	box(FL_DOWN_BOX);
}

void TimeRuler::draw()
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

	// Draw marks
	if (m_dIn != -1 && m_dOut != -1)
	{
		int S = X + value_to_x(m_dIn);
		int T = X + value_to_x(m_dOut);
		fl_color(FL_BLUE);
		fl_rectf(S, Y, T-S, H);
	}

	if (m_dIn != -1)
	{
		int S = X + value_to_x(m_dIn);
		fl_color(FL_DARK_BLUE);
		fl_line_style(FL_SOLID, 2);
		fl_line(S, Y, S, Y+H);
		fl_line_style(FL_SOLID, 1);
	}

	if (m_dOut != -1)
	{
		int T = X + value_to_x(m_dOut);
		fl_color(FL_DARK_BLUE);
		fl_line_style(FL_SOLID, 2);
		fl_line(T, Y, T, Y+H);
		fl_line_style(FL_SOLID, 1);
	}

	// Draw playhead
	int U = X + value_to_x(value());
	fl_color(FL_YELLOW);
	fl_line_style(FL_SOLID, 2);
	fl_line(U, Y, U, Y+H);
	fl_line_style(FL_SOLID, 1);
}

int TimeRuler::handle(int event)
{
	// Calculate area inside box
	int X = x() + Fl::box_dx(box());
	int Y = y() + Fl::box_dy(box());
	int W = w() - Fl::box_dw(box());
	int H = h() - Fl::box_dh(box());

	// Handle events
	switch (event)
	{
	case FL_PUSH:
		if (!Fl::event_inside(X, Y, W, H)) 
			return 0;
		handle_push();
		handle_drag(clamp(x_to_value(Fl::event_x() - X)));
		return 1;
	case FL_DRAG:
		handle_drag(clamp(x_to_value(Fl::event_x() - X)));
		return 1;
	case FL_RELEASE:
		handle_release();
		return 1;
	default:
		return 0;
	}
}

void TimeRuler::bounds(double min, double max)
{
	if (minimum() != min || maximum() != max)
	{
		Fl_Valuator::bounds(min, max); 
		damage(FL_DAMAGE_EXPOSE);
	}
}

void TimeRuler::marks(const double in, const double out)
{
	m_dIn = in;
	m_dOut = out;
	redraw();
}

int TimeRuler::value_to_x(const double v)
{
	double val;
	if (minimum() == maximum())
	{
		val = 0.0; //NOTE this conditional is still needed to avoid division by zero
	}
	else 
	{
		val = (v - minimum()) / (maximum() - minimum());
		if (val > 1.0) val = 1.0;
		else if (val < 0.0) val = 0.0;
	}

	const int W = w() - Fl::box_dw(box());
	return int(val * W + 0.5); //NOTE 0.5 is added for rounding purposes
}

double TimeRuler::x_to_value(const int x)
{
	const int W = w() - Fl::box_dw(box());
	const double val = x * (maximum() - minimum()) / W;
	return round(val + minimum());
}

} // namespace gui
} // namespace iola
