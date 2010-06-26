#ifndef IOLA_PROGRAMMONITOR_H
#define IOLA_PROGRAMMONITOR_H

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

class ProgramMonitor :
	public Fl_Group
{
public:
	ProgramMonitor(MainWindow* parent, int x, int y, int w, int h, const char *label = 0);
	~ProgramMonitor();

	int handle(int event);
	void stop();
        bool restart();
	void refresh();

private:
	MainWindow* m_pkParent;
	Mlt::Consumer* m_pkConsumer;

	Fl_Window* m_pkDisplay;

	void on_program_load();
	void on_program_playback();

	boost::signals2::connection on_program_load_connection;
	boost::signals2::connection on_program_playback_connection;

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->mark_out(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->play_forward(); }
	static void stop_playback(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->stop_playback(); }

	void mark_in();
	void mark_out();
	void mark_in_clear();
	void mark_out_clear();

	void step_backward();
	void step_forward();
	void play_backward();
	void play_forward();
	void stop_playback();
	void insert();
	void overwrite();

	Window xid();
};

#endif // IOLA_PROGRAMMONITOR_H
