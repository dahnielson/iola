/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_SOURCEMONITOR_H
#define IOLA_GUI_SOURCEMONITOR_H

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

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::ProgramMonitor

/// Source monitor to display and edit clips
class SourceMonitor :
	public Fl_Group
{
public:
	SourceMonitor(int x, int y, int w, int h, const char *label = 0);
	~SourceMonitor();

	/// FLTK event handler
	int handle(int event);
	/// Stop the source monitor consumer
	void stop();
	/// Restart the source monitor consumer
        bool restart();
	/// Refresh the source monitor consumer
	void refresh();

private:
	Mlt::Consumer* m_pkConsumer;
	Mlt::Event* m_pkFrameShowEvent;

	Fl_Window* m_pkDisplay;
	Fl_Hold_Browser* m_pkBrowser;
	TimeRuler* m_pkSlider;

	void on_sar_change();
	void on_dar_change();
	void on_par_change();
	void on_field_change();
	void on_fps_change();
	void on_sample_change();

	void on_source_load();
	void on_source_playback();
	void on_source_marks_change();

	boost::signals2::connection on_sar_change_connection;
	boost::signals2::connection on_dar_change_connection;
	boost::signals2::connection on_par_change_connection;
	boost::signals2::connection on_field_change_connection;
	boost::signals2::connection on_fps_change_connection;
	boost::signals2::connection on_sample_change_connection;
	boost::signals2::connection on_source_load_connection;
	boost::signals2::connection on_source_playback_connection;
	boost::signals2::connection on_source_marks_change_connection;
	boost::signals2::connection on_source_producer_change_connection;

	static void slider_callback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->slider_callback(); }
	static void browser_callback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->browser_callback(); }

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_out(); }
	static void clear_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_in_clear(); }
	static void clear_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_out_clear(); }
	static void step_backward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->step_backward(); }
	static void step_forward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->step_forward(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_forward(); }
	static void stop_playback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->stop_playback(); }
	static void goto_mark_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_in_goto(); }
	static void goto_mark_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_out_goto(); }

	static void frame_show_callback(mlt_consumer, SourceMonitor* self, mlt_frame frame_ptr)
	{
		Mlt::Frame frame(frame_ptr);
		self->frame_shown(frame);
	}

	void frame_shown(Mlt::Frame &frame);
	void on_source_producer_change();

	void slider_callback();

	void browser_load();
	void browser_callback();

	void mark_in();
	void mark_out();
	void mark_clip();
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

	Window xid();
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_SOURCEMONITOR_H
