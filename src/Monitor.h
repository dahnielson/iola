#ifndef IOLA_MONITOR_H
#define IOLA_MONITOR_H

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

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include "MainWindow.h"

class Monitor :
	public Fl_Group
{
public:
	Monitor(MainWindow* parent, int x, int y, int w, int h, const char *label = 0);
	~Monitor();

	void load(Mlt::Producer &producer);

private:
	pthread_mutex_t mutex;
	MainWindow* m_pkParent;

	Fl_Window* m_pkDisplay;
	Mlt::Consumer* m_pkConsumer;
	Mlt::Producer* m_pkProducer;

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<Monitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<Monitor*>(v)->mark_out(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<Monitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<Monitor*>(v)->play_forward(); }
	static void stop(Fl_Widget*, void* v) { reinterpret_cast<Monitor*>(v)->stop(); }

	void mark_in();
	void mark_out();
	void mark_in_clear();
	void mark_out_clear();

	void play_backward();
	void play_forward();
	void stop();

	Window xid();
        bool restart();
	void refresh();
	void set_speed(double speed);
	void seek(int pos);
};

#endif // IOLA_MONITOR_H
