#ifndef IOLA_MAINWINDOW_H
#define IOLA_MAINWINDOW_H

// :: melies iola
// Copyright (c) 2002-2010, Anders Dahnielson
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
class Monitor;

class MainWindow :
	public Fl_Double_Window
{
public:
	MainWindow();
	~MainWindow();

	Mlt::Profile& get_profile();

private:
	Fl_Menu_Bar* m_pkMenuBar;
	Monitor* m_pkSourceMonitor;
	Monitor* m_pkProgramMonitor;

	Mlt::Playlist* m_pkPlaylist;
	Mlt::Profile m_Profile;

	static void quit(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->quit(); }
	static void show_bin(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->show_bin(); }

	void quit();
	void show_bin();
};

#endif // IOLA_MAINWINDOW_H
