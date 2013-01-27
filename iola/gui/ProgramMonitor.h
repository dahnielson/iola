/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_PROGRAMMONITOR_H
#define IOLA_GUI_PROGRAMMONITOR_H

// Iola NLE
// Copyright (c) 2010-2013, Anders Dahnielson
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

// MLT
#include <mlt++/Mlt.h>

// QT
#include <QFrame>

// IOLA
#include <iola/model/imodel.h>
#include <iola/gui/imonitor.h>

namespace iola
{
namespace gui
{

class Monitor;
class TimeDisplay;

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::ProgramMonitor

/// Program monitor to display and edit sequences
class ProgramMonitor :
	public QFrame,
	public imonitor
{
	Q_OBJECT

public:
	ProgramMonitor(QWidget* parent = 0);
	~ProgramMonitor();

	///////////////////////////
	// iprogram_monitor

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

public slots:
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

	void goto_previous_edit();
	void goto_next_edit();
	void make_lift_edit();
	void make_extract_edit();
	void make_insert_edit();
	void make_overwrite_edit();

private:
	void on_playback_change();
	void on_marks_change(const int in, const int out);
	void on_producer_change(const int start, const int end);

	void frame_shown(Mlt::Frame &frame);

	Monitor* m_pkMonitor;
	TimeDisplay* m_pkCurrentTime;
	TimeDisplay* m_pkDurationTime;

	iola::model::imodel* m_pkModel;
	iola::model::iprogram* m_pkProgram;

	Mlt::Consumer* m_pkConsumer;
	Mlt::Event* m_pkFrameShowEvent;

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

	// Program change signals
	boost::signals2::connection on_playback_connection;
	boost::signals2::connection on_producer_change_connection;

	// MLT callback
	static void frame_show_callback(mlt_consumer, ProgramMonitor* self, mlt_frame frame_ptr)
	{
		Mlt::Frame frame(frame_ptr);
		self->frame_shown(frame);
	}
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_PROGRAMMONITOR_H
