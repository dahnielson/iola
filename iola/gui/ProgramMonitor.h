/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_PROGRAMMONITOR_H
#define IOLA_GUI_PROGRAMMONITOR_H

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
#include "TimeRuler.h"

namespace iola
{
namespace gui
{

class ProgramMonitor :
	public Fl_Group
{
public:
	ProgramMonitor(int x, int y, int w, int h, const char *label = 0);
	~ProgramMonitor();

	int handle(int event);
	void stop();
        bool restart();
	void refresh();

private:
	Mlt::Consumer* m_pkConsumer;
	Mlt::Event* m_pkFrameShowEvent;

	Fl_Window* m_pkDisplay;
	TimeRuler* m_pkSlider;

	void on_sar_change();
	void on_dar_change();
	void on_par_change();
	void on_field_change();
	void on_fps_change();
	void on_sample_change();

	void on_program_load();
	void on_program_playback();
	void on_program_marks_change();

	boost::signals2::connection on_sar_change_connection;
	boost::signals2::connection on_dar_change_connection;
	boost::signals2::connection on_par_change_connection;
	boost::signals2::connection on_field_change_connection;
	boost::signals2::connection on_fps_change_connection;
	boost::signals2::connection on_sample_change_connection;
	boost::signals2::connection on_program_load_connection;
	boost::signals2::connection on_program_playback_connection;
	boost::signals2::connection on_program_marks_change_connection;
	boost::signals2::connection on_program_producer_change_connection;

	static void slider_callback(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->slider_callback(); }

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->mark_out(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->play_forward(); }
	static void stop_playback(Fl_Widget*, void* v) { reinterpret_cast<ProgramMonitor*>(v)->stop_playback(); }

	static void frame_show_callback(mlt_consumer, ProgramMonitor* self, mlt_frame frame_ptr)
	{
		Mlt::Frame frame(frame_ptr);
		self->frame_shown(frame);
	}

	void frame_shown(Mlt::Frame &frame);
	void on_program_producer_change();

	void slider_callback();

	void mark_in();
	void mark_out();
	void mark_cut();
	void mark_in_clear();
	void mark_out_clear();
	void mark_in_goto();
	void mark_out_goto();

	void goto_start();
	void goto_end();

	void step_backward();
	void step_forward();
	void play_backward();
	void play_forward();
	void stop_playback();

	void edit_previous_goto();
	void edit_next_goto();

	void lift();
	void extract();
	void insert();
	void overwrite();

	Window xid();
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_PROGRAMMONITOR_H
