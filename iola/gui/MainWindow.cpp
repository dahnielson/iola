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

// BOOST
#include <boost/filesystem/fstream.hpp>

// STD
#include <iostream>
#include <sstream>

// IOLA
#include "MainWindow.h"
#include "ProgramMonitor.h"
#include "SourceMonitor.h"
#include <iola/dom/element_factory.h>
#include <iola/dom/root.h>
#include <iola/xml/parser.h>

namespace iola
{
namespace gui
{

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
	pkMenuBar->add("&File/&New", FL_CTRL+'n', (Fl_Callback *)new_project, this, 0);
	pkMenuBar->add("&File/&Open...", FL_CTRL+'o', (Fl_Callback *)open_project, this, 0);
	pkMenuBar->add("&File/&Save", FL_CTRL+'s', (Fl_Callback *)save_project, this, 0);
	pkMenuBar->add("&File/_Save &As...", 0, (Fl_Callback *)save_as_project, this, 0);
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

	// Project
	source_new();
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

void MainWindow::open_project()
{
	char* filename = fl_file_chooser("Open", "Sequence (*.xml)", NULL);
	if (filename)
	{
		m_kProjectPath = boost::filesystem::path(filename);
		program_load(m_kProjectPath);
	}
}

void MainWindow::save_project()
{
	if (m_kProjectPath.empty())
		save_as_project();
	else
		program_save(m_kProjectPath);
}

void MainWindow::save_as_project()
{
	char* filename = fl_file_chooser("Save", "Sequence (*.xml)", NULL);
	if (filename)
	{
		m_kProjectPath = boost::filesystem::path(filename);
		program_save(m_kProjectPath);
	}
}

void MainWindow::clear_project()
{
	source_new();
	program_new();
}

void MainWindow::quit_application()
{
	exit(0);
}

///////////////////////////////////////////
// Source

void MainWindow::source_new()
{
	rDebug("%s: Create a new source", __PRETTY_FUNCTION__);
	delete m_pkSource;
	m_pkSource = new Mlt::Playlist();
	m_pkSource->set_speed(0);
	m_pkSource->set("meta.iola.mark_in", -1);
	m_pkSource->set("meta.iola.mark_out", -1);
	on_source_load_signal();
	on_source_marks_change_signal();
}

void MainWindow::source_load(boost::filesystem::path clip)
{
	rDebug("%s: Load %s as source", __PRETTY_FUNCTION__, clip.string().c_str());
	m_pkSource->lock();
	m_pkSource->clear();
	Mlt::Producer* pkClipSource = new Mlt::Producer(m_Profile, clip.string().c_str());
	m_pkSource->append(*pkClipSource);
	m_pkSource->set_speed(0);
	m_pkSource->seek(0);
	m_pkSource->set("meta.iola.mark_in", -1);
	m_pkSource->set("meta.iola.mark_out", -1);
	m_pkSource->unlock();
	on_source_load_signal();
	on_source_marks_change_signal();
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
	if (m_pkSource && m_pkSource->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
		m_pkSource->lock();
		m_pkSource->set_speed(speed);
		m_pkSource->unlock();
	}
	on_source_playback_signal();
}

void MainWindow::source_seek(int position)
{
	if (m_pkSource)
	{
		rDebug("%s: Seek position at %i", __PRETTY_FUNCTION__, position);
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
		on_source_marks_change_signal();
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
		on_source_marks_change_signal();
	}
}

void MainWindow::source_set_mark_clip()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int in = m_pkSource->get_in();
		const int out = m_pkSource->get_out();
		rDebug("%s: Set source mark in at %i and out at %i", __PRETTY_FUNCTION__, in, out);
		m_pkSource->set("meta.iola.mark_in", in);
		m_pkSource->set("meta.iola.mark_out", out);
		m_pkSource->unlock();
		on_source_marks_change_signal();
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
		on_source_marks_change_signal();
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
		on_source_marks_change_signal();
	}
}

void MainWindow::source_goto_mark_in()
{
	if (m_pkSource)
	{
		const int mark = source_get_mark_in();
		rDebug("%s: Goto source mark in at %i", __PRETTY_FUNCTION__, mark);
		source_seek(mark);
	}
}

void MainWindow::source_goto_mark_out()
{
	if (m_pkSource)
	{
		const int mark = source_get_mark_out();
		rDebug("%s: Goto source mark out at %i", __PRETTY_FUNCTION__, mark);
		source_seek(mark);
	}
}

void MainWindow::source_goto_start()
{
	if (m_pkSource)
	{
		const int frame = m_pkSource->get_in();
		rDebug("%s: Goto source start at %i", __PRETTY_FUNCTION__, frame);
		source_seek(frame);
	}
}

void MainWindow::source_goto_end()
{
	if (m_pkSource)
	{
		const int frame = m_pkSource->get_out();
		rDebug("%s: Goto source end at %i", __PRETTY_FUNCTION__, frame);
		source_seek(frame);
	}
}

///////////////////////////////////////////
// Program

void MainWindow::program_new()
{
	rDebug("%s: Create a new program", __PRETTY_FUNCTION__);
	delete m_pkProgram;
	m_pkProgram = new Mlt::Playlist();
	m_pkProgram->set_speed(0);
	m_pkProgram->set("meta.iola.mark_in", -1);
	m_pkProgram->set("meta.iola.mark_out", -1);
	on_program_load_signal();
}

void MainWindow::program_load(boost::filesystem::path sequence)
{
	if (!m_pkProgram)
		return;

	rDebug("%s: Load %s as program", __PRETTY_FUNCTION__, sequence.string().c_str());
	iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
	iola::dom::root* pkRoot = new iola::dom::root();
	boost::filesystem::ifstream isXML(sequence);
	iola::xml::parser(pkFactory, pkRoot, isXML);
	delete m_pkProgram;
	m_pkProgram = new Mlt::Playlist();
	m_pkProgram->set_speed(0);
	m_pkProgram->set("meta.iola.mark_in", -1);
	m_pkProgram->set("meta.iola.mark_out", -1);
	pkRoot->restore();
	delete pkFactory;
        delete pkRoot;
	on_program_load_signal();
}

void MainWindow::program_save(boost::filesystem::path sequence) //FIXME
{
	if (!m_pkProgram)
		return;

	rDebug("%s: Save program as %s", __PRETTY_FUNCTION__, sequence.string().c_str());
	iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
	iola::dom::root* pkRoot = new iola::dom::root();

	// This is the easiest way to create a skeleton
	std::stringstream isXML;
	isXML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	isXML << "<!DOCTYPE iola>";
	isXML << "<iola version=\"0\">";
	isXML << "  <sequence>";
	isXML << "    <duration>0</duration>";
	isXML << "    <media>";
	isXML << "      <video><track></track></video>";
	isXML << "      <audio></audio>";
	isXML << "    </media>";
	isXML << "  </sequence>";
	isXML << "</iola>";

	// Parse the skeleton into an object
	iola::xml::parser(pkFactory, pkRoot, isXML);

	// Then call store to populate it
	pkRoot->store();

	// Output it as XML
	boost::filesystem::ofstream osXML(sequence);
	pkRoot->xml(osXML);

	delete pkFactory;
	delete pkRoot;
}

void MainWindow::program_set_duration(int duration)
{
	if (m_pkProgram && m_pkProgram->get_length() != duration)
	{
		rDebug("%s: Set duration to %i", __PRETTY_FUNCTION__, duration);
		if (m_pkProgram->get_length() > duration)
		{
			// Make shorter
			const int delta = m_pkProgram->get_length() - duration;
			m_pkProgram->remove_region(duration, delta);
		}
		else
		{
			// Make longer
			m_pkProgram->lock();
			m_pkProgram->blank(duration);
			m_pkProgram->unlock();
		}
	}
}

int MainWindow::program_get_duration()
{
	if (!m_pkProgram)
		return 0;

	m_pkProgram->lock();
	int duration = m_pkProgram->get_length();
	m_pkProgram->unlock();
	return duration;
}

double MainWindow::program_get_speed()
{
	if (!m_pkProgram)
		return 0;

	m_pkProgram->lock();
	double speed = m_pkProgram->get_speed();
	m_pkProgram->unlock();
	return speed;
}

void MainWindow::program_set_speed(double speed)
{
	if (m_pkProgram && m_pkProgram->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
		m_pkProgram->lock();
		m_pkProgram->set_speed(speed);
		m_pkProgram->unlock();
	}
	on_program_playback_signal();
}

void MainWindow::program_seek(int position)
{
	if (m_pkProgram)
	{
		rDebug("%s: Seek position at %i", __PRETTY_FUNCTION__, position);
		m_pkProgram->lock();
		m_pkProgram->seek(position < 0 ? 0 : position > m_pkProgram->get_out() ? m_pkProgram->get_out() : position);
		m_pkProgram->unlock();
	}
	on_program_seek_signal();
}

int MainWindow::program_get_clip_count()
{
	if (m_pkProgram)
		return m_pkProgram->count();
}

Mlt::ClipInfo* MainWindow::program_get_clip_info(int index)
{
	if (m_pkProgram)
		return m_pkProgram->clip_info(index);
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
		on_program_marks_change_signal();
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
		on_program_marks_change_signal();
	}
}

void MainWindow::program_set_mark_cut()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int in = m_pkProgram->clip(mlt_whence_relative_current, -1);
		const int out = m_pkProgram->clip(mlt_whence_relative_current, 1);
		rDebug("%s: Set program mark in at %i and out at %i", __PRETTY_FUNCTION__, in, out);
		m_pkProgram->set("meta.iola.mark_in", in);
		m_pkProgram->set("meta.iola.mark_out", out);
		m_pkProgram->unlock();
		on_program_marks_change_signal();
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
		on_program_marks_change_signal();
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
		on_program_marks_change_signal();
	}
}

void MainWindow::program_goto_mark_in()
{
	if (m_pkProgram)
	{
		const int mark = program_get_mark_in();
		rDebug("%s: Goto program mark in at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void MainWindow::program_goto_mark_out()
{
	if (m_pkProgram)
	{
		const int mark = program_get_mark_out();
		rDebug("%s: Goto program mark out at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void MainWindow::program_goto_previous_edit()
{
	if (m_pkProgram)
	{
		const int mark = m_pkProgram->clip(mlt_whence_relative_current, -1);
		rDebug("%s: Goto previous edit at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void MainWindow::program_goto_next_edit()
{
	if (m_pkProgram)
	{
		const int mark = m_pkProgram->clip(mlt_whence_relative_current, 1);
		rDebug("%s: Goto next edit at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void MainWindow::program_goto_start()
{
	if (m_pkProgram)
	{
		const int frame = m_pkProgram->get_in();
		rDebug("%s: Goto program start at %i", __PRETTY_FUNCTION__, frame);
		program_seek(frame);
	}
}

void MainWindow::program_goto_end()
{
	if (m_pkProgram)
	{
		const int frame = m_pkProgram->get_out();
		rDebug("%s: Goto program end at %i", __PRETTY_FUNCTION__, frame);
		program_seek(frame);
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
			rDebug("%s: Not good enough in/out points marked to perform three-point edit", __PRETTY_FUNCTION__);
			fl_beep();
			return;
		}

		Mlt::ClipInfo* pkInfo = m_pkSource->clip_info(0);
		program_insert(boost::filesystem::path(pkInfo->resource), program_in, source_in, source_out);
		Mlt::Playlist::delete_clip_info(pkInfo);
	}
}

void MainWindow::program_insert(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out)
{
	if (m_pkProgram)
	{
		rDebug("%s: Insert edit into playlist with %i clips: program_in=%i source_in=%i source_out=%i",
		       __PRETTY_FUNCTION__, m_pkProgram->count(), program_in, source_in, source_out);

		m_pkProgram->lock();
		const int clip_index = m_pkProgram->get_clip_index_at(program_in);
		m_pkProgram->split(clip_index, program_in - m_pkProgram->clip_start(clip_index));
		Mlt::Producer* pkClipSource = new Mlt::Producer(m_Profile, resource.string().c_str());
		m_pkProgram->insert(*pkClipSource, clip_index+1, source_in, source_out);
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
			rDebug("%s: Not good enough in/out points marked to perform three-point edit", __PRETTY_FUNCTION__);
			fl_beep();
			return;
		}

		Mlt::ClipInfo* pkInfo = m_pkSource->clip_info(0);
		program_overwrite(boost::filesystem::path(pkInfo->resource), program_in, source_in, source_out);
		Mlt::Playlist::delete_clip_info(pkInfo);
	}
}

void MainWindow::program_overwrite(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out)
{
	if (m_pkProgram)
	{
		rDebug("%s: Overwrite edit into playlist with %i clips: program_in=%i source_in=%i source_out=%i",
		       __PRETTY_FUNCTION__, m_pkProgram->count(), program_in, source_in, source_out);

		m_pkProgram->lock();
		const int clip_index = m_pkProgram->remove_region(program_in, source_out - source_in);
		Mlt::Producer* pkClipSource = new Mlt::Producer(m_Profile, resource.string().c_str());
		m_pkProgram->insert(*pkClipSource, clip_index, source_in, source_out);
		m_pkProgram->unlock();

		rDebug("%s: Playlist now contain %i clips", __PRETTY_FUNCTION__, m_pkProgram->count());
	}
}

} // namespace gui

namespace application
{

iola::gui::MainWindow* factory()
{
	if (!g_pkMainWindow)
		g_pkMainWindow = new iola::gui::MainWindow();

	return g_pkMainWindow;
}

} // namespace application
} // namespace iola
