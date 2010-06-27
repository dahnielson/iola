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

// RLOG
#include <rlog/rlog.h>

// STD
#include <iostream>

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

	// Program
	program_new();
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
	rDebug("%s: Load %s as source", __PRETTY_FUNCTION__, clip);
	pthread_mutex_lock(&mutex);
	delete m_pkSource;
	m_pkSource = new Mlt::Producer(m_Profile, clip);
	m_pkSource->set_speed(0);
	m_pkSource->set("meta.iola.mark_in", -1);
	m_pkSource->set("meta.iola.mark_out", -1);
//	m_pkSource->dump(); //debug
	pthread_mutex_unlock(&mutex);
	on_source_load_signal();
}

double MainWindow::source_get_speed()
{
	if (!m_pkSource)
		return 0;
		
	m_pkSource->lock();
	double speed = m_pkSource->get_speed();
	m_pkSource->unlock();
	return speed;
}

void MainWindow::source_set_speed(double speed)
{
	pthread_mutex_lock(&mutex);
	if (m_pkSource && m_pkSource->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
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
		rDebug("%s: Seek position x%i", __PRETTY_FUNCTION__, position);
		m_pkSource->lock();
		m_pkSource->seek(position < 0 ? 0 : position > m_pkSource->get_out() ? m_pkSource->get_out() : position);
		m_pkSource->unlock();
	}
	on_source_seek_signal();
}

void MainWindow::source_step_forward()
{
	if (!m_pkSource)
		return;

	source_set_speed(0);
	int position = m_pkSource->frame();
	source_seek(++position);
}

void MainWindow::source_step_backward()
{
	if (!m_pkSource)
		return;

	source_set_speed(0);
	int position = m_pkSource->frame();
	source_seek(--position);
}

void MainWindow::source_play_forward()
{
	if (!m_pkSource)
		return;

	double speed = m_pkSource->get_speed();
	if (speed == 0 || speed + 1 == 0)
		speed = 1;
	else if (speed < 0)
		speed /= 2;
	else if (speed < 8)
		speed *= 2;
	source_set_speed(speed);
}

void MainWindow::source_play_reverse()
{
	if (!m_pkSource)
		return;

	double speed = m_pkSource->get_speed();
	if (speed == 0 || speed - 1 == 0)
		speed = -1;
	else if (speed > 0)
		speed /= 2;
	else if (speed > -8)
		speed *= 2;
	source_set_speed(speed);
}

void MainWindow::source_pause()
{
	source_set_speed(0);
}

int MainWindow::source_get_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int mark = m_pkSource->get_int("meta.iola.mark_in");
		m_pkSource->unlock();
		return mark;
	}
}

int MainWindow::source_get_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int mark = m_pkSource->get_int("meta.iola.mark_out");
		m_pkSource->unlock();
		return mark;
	}
}

void MainWindow::source_set_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Set source mark in at %i", __PRETTY_FUNCTION__, m_pkSource->frame());
		m_pkSource->set("meta.iola.mark_in", m_pkSource->frame());
		m_pkSource->unlock();
	}
}

void MainWindow::source_set_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Set source mark out at %i", __PRETTY_FUNCTION__, m_pkSource->frame());
		m_pkSource->set("meta.iola.mark_out", m_pkSource->frame());
		m_pkSource->unlock();
	}
}

void MainWindow::source_clear_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Clear source mark in", __PRETTY_FUNCTION__);
		m_pkSource->set("meta.iola.mark_in", -1);
		m_pkSource->unlock();
	}
}

void MainWindow::source_clear_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Clear source mark out", __PRETTY_FUNCTION__);
		m_pkSource->set("meta.iola.mark_out", -1);
		m_pkSource->unlock();
	}
}

///////////////////////////////////////////
// Program

void MainWindow::program_new()
{
	rDebug("%s: Create a new program", __PRETTY_FUNCTION__);
	pthread_mutex_lock(&mutex);
	delete m_pkProgram;
	m_pkProgram = new Mlt::Playlist();
	m_pkProgram->set_speed(0);
	m_pkProgram->set("meta.iola.mark_in", -1);
	m_pkProgram->set("meta.iola.mark_out", -1);
//	m_pkProgram->dump(); //debug
	pthread_mutex_unlock(&mutex);
	on_program_load_signal();
}


double MainWindow::program_get_speed()
{
	m_pkProgram->lock();
	double speed = m_pkProgram->get_speed();
	m_pkProgram->unlock();
	return speed;
}

void MainWindow::program_set_speed(double speed)
{
	pthread_mutex_lock(&mutex);
	if (m_pkProgram && m_pkProgram->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
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
		rDebug("%s: Seek position x%i", __PRETTY_FUNCTION__, position);
		m_pkProgram->lock();
		m_pkProgram->seek(position < 0 ? 0 : position > m_pkProgram->get_out() ? m_pkProgram->get_out() : position);
		m_pkProgram->unlock();
	}
	on_program_seek_signal();
}

void MainWindow::program_step_forward()
{
	if (!m_pkProgram)
		return;

	program_set_speed(0);
	int position = m_pkProgram->frame();
	program_seek(++position);
}

void MainWindow::program_step_backward()
{
	if (!m_pkProgram)
		return;

	program_set_speed(0);
	int position = m_pkProgram->frame();
	program_seek(--position);
}

void MainWindow::program_play_forward()
{
	if (!m_pkProgram)
		return;

	double speed = m_pkProgram->get_speed();
	if (speed == 0 || speed + 1 == 0)
		speed = 1;
	else if (speed < 0)
		speed /= 2;
	else if (speed < 8)
		speed *= 2;
	program_set_speed(speed);
}

void MainWindow::program_play_reverse()
{
	if (!m_pkProgram)
		return;

	double speed = m_pkProgram->get_speed();
	if (speed == 0 || speed - 1 == 0)
		speed = -1;
	else if (speed > 0)
		speed /= 2;
	else if (speed > -8)
		speed *= 2;
	program_set_speed(speed);
}

void MainWindow::program_pause()
{
	program_set_speed(0);
}

int MainWindow::program_get_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int mark = m_pkProgram->get_int("meta.iola.mark_in");
		m_pkProgram->unlock();
		return mark;
	}
}

int MainWindow::program_get_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int mark = m_pkProgram->get_int("meta.iola.mark_out");
		m_pkProgram->unlock();
		return mark;
	}}

void MainWindow::program_set_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Set program mark in at %i", __PRETTY_FUNCTION__, m_pkProgram->frame());
		m_pkProgram->set("meta.iola.mark_in", m_pkProgram->frame());
		m_pkProgram->unlock();
	}
}

void MainWindow::program_set_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Set program mark out at %i", __PRETTY_FUNCTION__, m_pkProgram->frame());
		m_pkProgram->set("meta.iola.mark_out", m_pkProgram->frame());
		m_pkProgram->unlock();
	}
}

void MainWindow::program_clear_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Clear program mark in", __PRETTY_FUNCTION__);
		m_pkProgram->set("meta.iola.mark_in", -1);
		m_pkProgram->unlock();
	}
}

void MainWindow::program_clear_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Clear program mark out", __PRETTY_FUNCTION__);
		m_pkProgram->set("meta.iola.mark_out", -1);
		m_pkProgram->unlock();
	}
}

void MainWindow::program_insert()
{
	if (!m_pkProgram)
		rWarning("%s: m_pkProgram is NULL", __PRETTY_FUNCTION__);

	if (!m_pkSource)
		rWarning("%s: m_pkSource is NULL", __PRETTY_FUNCTION__);

	if (m_pkProgram && m_pkSource)
	{
		int program_in, source_in, source_out;

		if (program_get_mark_in() != -1 && program_get_mark_out() != -1 && 
		    source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
		    program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Four-point editing: program in/out takes precedence", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() == -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source in marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() == -1 && source_get_mark_out() != -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source out marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_out() - duration;
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() == -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in and source in/out marked", __PRETTY_FUNCTION__);
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() == -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program out and source in/out marked", __PRETTY_FUNCTION__);
			const int duration = source_get_mark_out() - source_get_mark_in();
			program_in = program_get_mark_out() - duration;
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else
		{
			rDebug("%s: Not goo enough in/out points marked to perform three-point edit", __PRETTY_FUNCTION__);
			return;
		}

		rDebug("%s: Insert edit into playlist with %i clipes", __PRETTY_FUNCTION__, m_pkProgram->count());
		rDebug("%s: program_in=%i source_in=%i source_out=%i",  __PRETTY_FUNCTION__, program_in, source_in, source_out);
		m_pkProgram->lock();
		const int clip_index = m_pkProgram->get_clip_index_at(program_in);
		m_pkProgram->split(clip_index, program_in - m_pkProgram->clip_start(clip_index));
		m_pkProgram->insert(*m_pkSource, clip_index+1, source_in, source_out);
		m_pkProgram->unlock();
		rDebug("%s: Playlist now contain %i clips", __PRETTY_FUNCTION__, m_pkProgram->count());
	}
}

void MainWindow::program_overwrite()
{
	if (!m_pkProgram)
		rWarning("%s: m_pkProgram is NULL", __PRETTY_FUNCTION__);

	if (!m_pkSource)
		rWarning("%s: m_pkSource is NULL", __PRETTY_FUNCTION__);

	if (m_pkProgram && m_pkSource)
	{
		int program_in, source_in, source_out;

		if (program_get_mark_in() != -1 && program_get_mark_out() != -1 && 
		    source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
		    program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Four-point editing: program in/out takes precedence", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() == -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source in marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() == -1 && source_get_mark_out() != -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source out marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_out() - duration;
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() == -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in and source in/out marked", __PRETTY_FUNCTION__);
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() == -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program out and source in/out marked", __PRETTY_FUNCTION__);
			const int duration = source_get_mark_out() - source_get_mark_in();
			program_in = program_get_mark_out() - duration;
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else
		{
			rDebug("%s: To few in/out marked to perform three-point edit", __PRETTY_FUNCTION__);
			return;
		}

		rDebug("%s: Overwrite edit into playlist with %i clipes", __PRETTY_FUNCTION__, m_pkProgram->count());
		rDebug("%s: program_in=%i source_in=%i source_out=%i",  __PRETTY_FUNCTION__, program_in, source_in, source_out);
		m_pkProgram->lock();
		const int clip_index = m_pkProgram->remove_region(program_in, source_out - source_in);
		m_pkProgram->insert(*m_pkSource, clip_index, source_in, source_out);
		m_pkProgram->unlock();
		rDebug("%s: Playlist now contain %i clips", __PRETTY_FUNCTION__, m_pkProgram->count());
	}
}

