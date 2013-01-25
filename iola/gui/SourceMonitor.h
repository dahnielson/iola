/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
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

// BOOST
#include <boost/signals2.hpp>

// FLTK
#include "fltk.h"

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include <iola/model/imodel.h>
#include <iola/gui/imonitor.h>

#include "TimeDisplay.h"
#include "TimeRuler.h"

namespace iola
{
namespace gui
{

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::SourceMonitor

/// Source monitor to display and edit clips
class SourceMonitor :
	public imonitor,
	public Fl_Group
{
public:
	SourceMonitor(int x, int y, int w, int h, const char* label = 0);
	~SourceMonitor();

	///////////////////////////
	// isource_monitor

	void connect_to(iola::model::imodel* model);

	///////////////////////////
	// ivideo_characteristics

	void set_width(const int width);
	void set_height(const int height);
	void set_dar(const boost::rational<int> dar);
	void set_par(const boost::rational<int> par);
	void set_fps(const boost::rational<int> fps);
	void set_timebase(const int timebase);
	void set_ntsc(const bool ntsc);
	void set_progressive(const bool progressive);

	///////////////////////////
	// iaudio_characteristics

	void set_sample_depth(const int depth);
	void set_sample_rate(const int rate);

	/// FLTK event handler
	int handle(int event);
	/// Stop the source monitor consumer
	void stop();
	/// Restart the source monitor consumer
        bool restart();
	/// Refresh the source monitor consumer
	void refresh();

private:
	void on_playback_change();
	void on_marks_change(const int in, const int out);
	void on_producer_change(const int start, const int end);

	void frame_shown(Mlt::Frame &frame);

	void browser_load();
	void browser_callback();

	void slider_callback();

	void mark_in();
	void mark_out();
	void mark_clip();
	void clear_mark_in();
	void clear_mark_out();
	void goto_mark_in();
	void goto_mark_out();

	void goto_start();
	void goto_end();
	void play_backward();
	void play_forward();
	void step_backward();
	void step_forward();
	void stop_playback();

	Window xid();

	iola::model::isource* m_pkSource;

	Mlt::Consumer* m_pkConsumer;
	Mlt::Event* m_pkFrameShowEvent;

	TimeRuler* m_pkSlider;
	TimeDisplay* m_pkTimecode;
	Fl_Window* m_pkDisplay;
	Fl_Hold_Browser* m_pkBrowser;

	// Video settings change signals
	boost::signals2::connection on_width_change_connection;
	boost::signals2::connection on_height_change_connection;
	boost::signals2::connection on_dar_change_connection;
	boost::signals2::connection on_par_change_connection;
	boost::signals2::connection on_progressive_change_connection;
	boost::signals2::connection on_fps_change_connection;
	boost::signals2::connection on_timebase_change_connection;
	boost::signals2::connection on_ntsc_change_connection;

	// Audio settings change signals
	boost::signals2::connection on_sample_depth_change_connection;
	boost::signals2::connection on_sample_rate_change_connection;

	// Markers change signal
	boost::signals2::connection on_marks_change_connection;

	// Source change signals
	boost::signals2::connection on_playback_connection;
	boost::signals2::connection on_producer_change_connection;

	// FLTK callbacks
	static void slider_callback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->slider_callback(); }
	static void browser_callback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->browser_callback(); }

	static void mark_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_in(); }
	static void mark_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->mark_out(); }
	static void clear_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->clear_mark_in(); }
	static void clear_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->clear_mark_out(); }
	static void step_backward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->step_backward(); }
	static void step_forward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->step_forward(); }
	static void play_backward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_backward(); }
	static void play_forward(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->play_forward(); }
	static void stop_playback(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->stop_playback(); }
	static void goto_mark_in(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->goto_mark_in(); }
	static void goto_mark_out(Fl_Widget*, void* v) { reinterpret_cast<SourceMonitor*>(v)->goto_mark_out(); }

	// MLT callback
	static void frame_show_callback(mlt_consumer, SourceMonitor* self, mlt_frame frame_ptr)
	{
		Mlt::Frame frame(frame_ptr);
		self->frame_shown(frame);
	}
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_SOURCEMONITOR_H
