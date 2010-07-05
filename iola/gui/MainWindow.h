#ifndef IOLA_MAINWINDOW_H
#define IOLA_MAINWINDOW_H

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

// BOOST
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

// FLTK
#include "fltk.h"

// MLT
#include <mlt++/Mlt.h>

namespace iola
{
namespace gui
{

class ProgramMonitor;
class SourceMonitor;

class MainWindow :
	public Fl_Double_Window
{
public:
	MainWindow();
	~MainWindow();

	Mlt::Profile& get_profile();
	Mlt::Producer& get_source();
	Mlt::Producer& get_program();

	void open_project();
	void save_project();
	void save_as_project();
	void clear_project();
	void quit_application();

	void source_new();
	void source_load(boost::filesystem::path clip);
	double source_get_speed();
	void source_set_speed(double speed);
	void source_seek(int position);

	void source_step_forward();
	void source_step_backward();
	void source_play_forward();
	void source_play_reverse();
	void source_pause();
	int source_get_mark_in();
	int source_get_mark_out();
	void source_set_mark_in();
	void source_set_mark_out();
	void source_set_mark_clip();
	void source_clear_mark_in();
	void source_clear_mark_out();
	void source_goto_mark_in();
	void source_goto_mark_out();
	void source_goto_start();
	void source_goto_end();

	void program_new();
	void program_load(boost::filesystem::path sequence);
	void program_save(boost::filesystem::path sequence);
	void program_set_duration(int duration);
	int program_get_duration();
	void program_set_speed(double speed);
	double program_get_speed();
	void program_seek(int position);

	int program_get_clip_count();
	Mlt::ClipInfo* program_get_clip_info(int index);

	void program_step_forward();
	void program_step_backward();
	void program_play_forward();
	void program_play_reverse();
	void program_pause();
	int program_get_mark_in();
	int program_get_mark_out();
	void program_set_mark_in();
	void program_set_mark_out();
	void program_set_mark_cut();
	void program_clear_mark_in();
	void program_clear_mark_out();
	void program_goto_mark_in();
	void program_goto_mark_out();
	void program_goto_previous_edit();
	void program_goto_next_edit();
	void program_goto_start();
	void program_goto_end();
	void program_insert();
	void program_insert(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out);
	void program_overwrite();
	void program_overwrite(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out);

	typedef boost::signals2::signal<void ()> signal_t;

	signal_t on_source_load_signal;
	signal_t on_source_playback_signal;
	signal_t on_source_seek_signal;
	signal_t on_source_marks_change_signal;

	signal_t on_program_load_signal;
	signal_t on_program_playback_signal;
	signal_t on_program_seek_signal;
	signal_t on_program_marks_change_signal;

private:
	pthread_mutex_t mutex;

	// The "model"
	Mlt::Playlist* m_pkSource;
	Mlt::Playlist* m_pkProgram;
	Mlt::Profile m_Profile;

	SourceMonitor* m_pkSourceMonitor;
	ProgramMonitor* m_pkProgramMonitor;

	boost::filesystem::path m_kProjectPath;

	static void new_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->clear_project(); }
	static void open_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->open_project(); }
	static void save_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->save_project(); }
	static void save_as_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->save_as_project(); }
	static void quit_application(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->quit_application(); }
};

} // namespace gui
} // namespace iola

namespace { static iola::gui::MainWindow* g_pkMainWindow; }
namespace iola { namespace application { iola::gui::MainWindow* factory(); } }

#endif // IOLA_MAINWINDOW_H
