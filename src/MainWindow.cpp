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

// IOLA
#include "MainWindow.h"
#include "ProgramMonitor.h"
#include "SourceMonitor.h"

MainWindow::MainWindow() :
	Fl_Double_Window(Fl::w(), Fl::h(), "Iola NLE"),
	m_pkSource(0),
	m_pkProgram(0)
{
	pthread_mutex_init(&mutex, NULL);

	// Menu
	Fl_Menu_Bar* pkMenuBar = new Fl_Menu_Bar(0, 0, Fl::w(), 25);
	pkMenuBar->clear();
	pkMenuBar->box(FL_FLAT_BOX);
	pkMenuBar->textsize(11);
	pkMenuBar->textcolor(FL_FOREGROUND_COLOR);
	pkMenuBar->color(FL_BACKGROUND_COLOR, FL_SELECTION_COLOR);
//	pkMenuBar->add("&File/&New", FL_CTRL+'n', 0, this, 0);
//	pkMenuBar->add("&File/&Open...", FL_CTRL+'o', 0, this, 0);
//	pkMenuBar->add("&File/&Save", FL_CTRL+'s', 0, this, 0);
//	pkMenuBar->add("&File/Save &As...", 0, 0, this, 0);
	pkMenuBar->add("&File/&Quit", FL_CTRL+'q', (Fl_Callback *)quit_application, this, 0);
//	pkMenuBar->add("&Edit/&Undo", FL_CTRL+'z', 0, this, 0);
//	pkMenuBar->add("&Edit/&Redo", FL_CTRL+'y', 0, this, 0);
//	pkMenuBar->add("&Help/&About Iola...", 0, 0, this, 0);

	// Layout
	Fl_Pack* pkMainGroup = new Fl_Pack(0, 25, w(), h()-25);
	Fl_Tile* pkDesktopGroup = new Fl_Tile(0, 0, w(), pkMainGroup->h());
	Fl_Group* pkMonitorGroup = new Fl_Group(0, 0, pkDesktopGroup->w(), pkDesktopGroup->h()/5*3);

	m_pkSourceMonitor = new SourceMonitor(this, 0, 0, pkMonitorGroup->w()/2, pkMonitorGroup->h());
	m_pkProgramMonitor = new ProgramMonitor(this, pkMonitorGroup->w()/2, 0, pkMonitorGroup->w()/2, pkMonitorGroup->h());
	Fl_Box* timeline = new Fl_Box(FL_THIN_UP_BOX, 0, pkMonitorGroup->h(), w(), pkDesktopGroup->h()-pkMonitorGroup->h(), "");

	pkMonitorGroup->type(Fl_Pack::HORIZONTAL);
	pkMonitorGroup->box(FL_NO_BOX);
	pkMonitorGroup->add(m_pkSourceMonitor);
	pkMonitorGroup->add(m_pkProgramMonitor);
	pkMonitorGroup->end();

	pkDesktopGroup->box(FL_NO_BOX);
	pkDesktopGroup->add(pkMonitorGroup);
	pkDesktopGroup->add(timeline);
	pkDesktopGroup->end();

	pkMainGroup->add(pkDesktopGroup);
	pkMainGroup->resizable(pkDesktopGroup);
	pkMainGroup->end();

	// Window
	xclass("iola");
	resizable(pkMainGroup);
	size_range(800, 750);
	end();
}

MainWindow::~MainWindow()
{
	delete m_pkSource;
	delete m_pkProgram;
	pthread_mutex_destroy(&mutex);
}

Mlt::Profile& MainWindow::get_profile()
{
	return m_Profile;
}

Mlt::Producer& MainWindow::get_source()
{
	return *m_pkSource;
}

Mlt::Producer& MainWindow::get_program()
{
	return *m_pkProgram;
}

void MainWindow::quit_application()
{
	exit(0);
}

///////////////////////////////////////////
// Source

void MainWindow::source_load(const char* clip)
{
	pthread_mutex_lock(&mutex);
	delete m_pkSource;
	m_pkSource = new Mlt::Producer(m_Profile, clip);
	m_pkSource->set_speed(0);
	m_pkSource->set("meta.iola.mark_in", 0);
	m_pkSource->set("meta.iola.mark_out", m_pkSource->get_length()-1);
//	m_pkSource->dump(); //debug
	pthread_mutex_unlock(&mutex);
	on_source_load_signal();
}

void MainWindow::source_set_speed(double speed)
{
	pthread_mutex_lock(&mutex);
	if (m_pkSource && m_pkSource->get_speed() != speed)
	{
		m_pkSource->lock();
		m_pkSource->set_speed(speed);
		m_pkSource->unlock();
	}
	pthread_mutex_unlock(&mutex);
	on_source_playback_signal();
}

void MainWindow::source_seek(int position)
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		m_pkSource->seek(position < 0 ? 0 : position > m_pkSource->get_out() ? m_pkSource->get_out() : position);
		m_pkSource->unlock();
	}
	on_source_seek_signal();
}

void MainWindow::source_play_forward()
{
	source_set_speed(1);
}

void MainWindow::source_play_reverse()
{
	source_set_speed(-1);
}

void MainWindow::source_pause()
{
	source_set_speed(0);
}

void MainWindow::source_set_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		m_pkSource->set("meta.iola.mark_in", m_pkSource->frame());
		m_pkSource->unlock();
	}
}

void MainWindow::source_set_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		m_pkSource->set("meta.iola.mark_out", m_pkSource->frame());
		m_pkSource->unlock();
	}
}

void MainWindow::source_clear_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		m_pkSource->set("meta.iola.mark_in", 0);
		m_pkSource->unlock();
	}
}

void MainWindow::source_clear_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		m_pkSource->set("meta.iola.mark_out", m_pkSource->get_length()-1);
		m_pkSource->unlock();
	}
}

///////////////////////////////////////////
// Program

void MainWindow::program_set_speed(double speed)
{
	pthread_mutex_lock(&mutex);
	if (m_pkProgram && m_pkProgram->get_speed() != speed)
	{
		m_pkProgram->lock();
		m_pkProgram->set_speed(speed);
		m_pkProgram->unlock();
	}
	pthread_mutex_unlock(&mutex);
	on_program_playback_signal();
}

void MainWindow::program_seek(int position)
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		m_pkProgram->seek(position < 0 ? 0 : position > m_pkProgram->get_out() ? m_pkProgram->get_out() : position);
		m_pkProgram->unlock();
	}
	on_program_seek_signal();
}

void MainWindow::program_play_forward()
{
	program_set_speed(1);
}

void MainWindow::program_play_reverse()
{
	program_set_speed(-1);
}

void MainWindow::program_pause()
{
	program_set_speed(0);
}

void MainWindow::program_set_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		m_pkProgram->set("meta.iola.mark_in", m_pkProgram->frame());
		m_pkProgram->unlock();
	}
}

void MainWindow::program_set_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		m_pkProgram->set("meta.iola.mark_out", m_pkProgram->frame());
		m_pkProgram->unlock();
	}
}

void MainWindow::program_clear_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		m_pkProgram->set("meta.iola.mark_in", 0);
		m_pkProgram->unlock();
	}
}

void MainWindow::program_clear_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		m_pkProgram->set("meta.iola.mark_out", m_pkProgram->get_length()-1);
		m_pkProgram->unlock();
	}
}

void MainWindow::program_insert()
{
	if (m_pkProgram && m_pkSource)
	{
		//todo: edit playlist
	}
}

void MainWindow::program_overwrite()
{
	if (m_pkProgram && m_pkSource)
	{
		//todo: edit playlist
	}
}

