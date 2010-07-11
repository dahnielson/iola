/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_TIMERULER_H
#define IOLA_GUI_TIMERULER_H

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

#include <Fl/Fl_Valuator.H>

namespace iola
{
namespace gui
{

class TimeRuler :
	public Fl_Valuator
{
public:
	TimeRuler(int x, int y, int w, int h, const char *label = 0);

	void draw();
	int handle(int event);

	void bounds(double min, double max);
	void marks(double in, double out);

private:
	double m_dIn;
	double m_dOut;

	int value_to_x(const double v);
	double x_to_value(const int x);
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_TIMERULER_H
