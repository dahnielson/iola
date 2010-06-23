#ifndef IOLA_SOURCEMONITOR_H
#define IOLA_SOURCEMONITOR_H

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

class SourceMonitor :
	public Fl_Group
{
public:
	SourceMonitor(MainWindow* parent, int x, int y, int w, int h, const char *label = 0);
	~SourceMonitor();

	int handle(int event);

private:
	MainWindow* m_pkParent;
	Mlt::Consumer* m_pkConsumer;

	Fl_Window* m_pkDisplay;
	Fl_Hold_Browser* m_pkBrowser;

	void on_source_load();
	void on_source_playback();

	boost::signals2::connection on_source_load_connection;
	boost::signals2::connection on_source_playback_connection;

	static void browser_callback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->browser_callback(); }

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_out(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_forward(); }
	static void stop(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->stop(); }

	void browser_load();
	void browser_callback();

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
};

#endif // IOLA_SOURCEMONITOR_H
