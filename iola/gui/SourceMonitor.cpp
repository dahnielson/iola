/* -*- Mode: C++ ; c-basic-offset: 8 -*- */

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

// STD
#include <cassert>
#include <exception>
#include <iostream>

// BOOST
#include <boost/filesystem.hpp>

// RLOG
#include <rlog/rlog.h>

// IOLA
#include <iola/model/isource.h>
#include "SourceMonitor.h"

// XBM
#include "xbm/clear_in.xbm"
#include "xbm/clear_out.xbm"
#include "xbm/mark_in.xbm"
#include "xbm/mark_out.xbm"
#include "xbm/pause.xbm"
#include "xbm/play_fwd.xbm"
#include "xbm/play_rev.xbm"
#include "xbm/step_fwd.xbm"
#include "xbm/step_rev.xbm"
#include "xbm/goto_mark_in.xbm"
#include "xbm/goto_mark_out.xbm"

namespace iola
{
namespace gui
{

SourceMonitor::SourceMonitor(int x, int y, int w, int h, const char* label) :
	Fl_Group(x, y, w, h, label),
	m_pkSource(0),
	m_pkConsumer(0),
	m_pkFrameShowEvent(0),
	m_pkSlider(0),
	m_pkTimecode(0),
	m_pkDisplay(0),
	m_pkBrowser(0)
{
	// Transport Slider
	m_pkSlider = new TimeRuler(x+4, y+h-50, w-8, 19);
	assert(m_pkSlider);
	m_pkSlider->bounds(0, 0);
	m_pkSlider->precision(0);
	m_pkSlider->callback((Fl_Callback *)slider_callback, this);
	
	// Transport Button Group
	Fl_Pack *pkTransportGroup = new Fl_Pack(x+4, y+h-28, w-8, 25);
	pkTransportGroup->box(FL_NO_BOX);
	pkTransportGroup->type(Fl_Pack::HORIZONTAL);

	Fl_Box* pkLeftFiller = new Fl_Box(0, 0, (w-275)/2, 25);

	Fl_Bitmap* xbmGotoIn = new Fl_Bitmap(goto_mark_in_bits, goto_mark_in_width, goto_mark_in_height);
	Fl_Button* pkGotoIn = new Fl_Button(0, 0, 25, 25);
	pkGotoIn->image(xbmGotoIn);
	pkGotoIn->type(FL_NORMAL_BUTTON);
	pkGotoIn->callback((Fl_Callback *)goto_mark_in, this);

	Fl_Bitmap* xbmClearIn = new Fl_Bitmap(clear_in_bits, clear_in_width, clear_in_height);
	Fl_Button* pkClearIn = new Fl_Button(0, 0, 25, 25);
	pkClearIn->image(xbmClearIn);
	pkClearIn->type(FL_NORMAL_BUTTON);
	pkClearIn->callback((Fl_Callback *)clear_in, this);

	Fl_Bitmap* xbmMarkIn = new Fl_Bitmap(mark_in_bits, mark_in_width, mark_in_height);
	Fl_Button* pkMarkIn = new Fl_Button(0, 0, 25, 25);
	pkMarkIn->image(xbmMarkIn);
	pkMarkIn->type(FL_NORMAL_BUTTON);
	pkMarkIn->callback((Fl_Callback *)mark_in, this);

	Fl_Bitmap* xbmPlayRev = new Fl_Bitmap(play_rev_bits, play_rev_width, play_rev_height);
	Fl_Button* pkPlayBackward = new Fl_Button(0, 0, 25, 25);
	pkPlayBackward->image(xbmPlayRev);
	pkPlayBackward->type(FL_NORMAL_BUTTON);
	pkPlayBackward->callback((Fl_Callback *)play_backward, this);

	Fl_Bitmap* xbmStepRev = new Fl_Bitmap(step_rev_bits, step_rev_width, step_rev_height);
	Fl_Button* pkStepBackward = new Fl_Button(0, 0, 25, 25);
	pkStepBackward->image(xbmStepRev);
	pkStepBackward->type(FL_NORMAL_BUTTON);
	pkStepBackward->callback((Fl_Callback *)step_backward, this);

	Fl_Bitmap* xbmPause = new Fl_Bitmap(pause_bits, pause_width, pause_height);
	Fl_Button* pkStop = new Fl_Button(0, 0, 25, 25);
	pkStop->image(xbmPause);
	pkStop->type(FL_NORMAL_BUTTON);
	pkStop->callback((Fl_Callback *)stop_playback, this);

	Fl_Bitmap* xbmStepFwd = new Fl_Bitmap(step_fwd_bits, step_fwd_width, step_fwd_height);
	Fl_Button* pkStepForward = new Fl_Button(0, 0, 25, 25);
	pkStepForward->image(xbmStepFwd);
	pkStepForward->type(FL_NORMAL_BUTTON);
	pkStepForward->callback((Fl_Callback *)step_forward, this);

	Fl_Bitmap* xbmPlayFwd = new Fl_Bitmap(play_fwd_bits, play_fwd_width, play_fwd_height);
	Fl_Button* pkPlayForward = new Fl_Button(0, 0, 25, 25);
	pkPlayForward->image(xbmPlayFwd);
	pkPlayForward->type(FL_NORMAL_BUTTON);
	pkPlayForward->callback((Fl_Callback *)play_forward, this);

	Fl_Bitmap* xbmMarkOut = new Fl_Bitmap(mark_out_bits, mark_out_width, mark_out_height);
	Fl_Button* pkMarkOut = new Fl_Button(0, 0, 25, 25);
	pkMarkOut->image(xbmMarkOut);
	pkMarkOut->type(FL_NORMAL_BUTTON);
	pkMarkOut->callback((Fl_Callback *)mark_out, this);

	Fl_Bitmap* xbmClearOut = new Fl_Bitmap(clear_out_bits, clear_out_width, clear_out_height);
	Fl_Button* pkClearOut = new Fl_Button(0, 0, 25, 25);
	pkClearOut->image(xbmClearOut);
	pkClearOut->type(FL_NORMAL_BUTTON);
	pkClearOut->callback((Fl_Callback *)clear_in, this);

	Fl_Bitmap* xbmGotoOut = new Fl_Bitmap(goto_mark_out_bits, goto_mark_out_width, goto_mark_out_height);
	Fl_Button* pkGotoOut = new Fl_Button(0, 0, 25, 25);
	pkGotoOut->image(xbmGotoOut);
	pkGotoOut->type(FL_NORMAL_BUTTON);
	pkGotoOut->callback((Fl_Callback *)goto_mark_out, this);

	Fl_Box* pkRightFiller = new Fl_Box(0, 0, ((w-275)/2)-98, 25);

	m_pkTimecode = new TimeDisplay(0, 0, 90, 20);
	assert(m_pkTimecode);

	pkTransportGroup->end();

	// Main Group
	Fl_Group *pkMainGroup = new Fl_Group(x, y+20, w, h-25, "Source");
	pkMainGroup->labelsize(11);
	m_pkDisplay = new Fl_Window(x+4, y+30, w-8, h-82);
	assert(m_pkDisplay);
	m_pkDisplay->color(FL_BLACK);
	m_pkDisplay->end();
	pkMainGroup->add(m_pkDisplay);
	pkMainGroup->add(m_pkSlider);
	pkMainGroup->add(pkTransportGroup);
	pkMainGroup->resizable(m_pkDisplay);
	pkMainGroup->end();

	// Bin Group
	Fl_Group* pkBinGroup = new Fl_Group(x, y+20, w, h-25, "Bin");
	pkBinGroup->labelsize(11);
	m_pkBrowser = new Fl_Hold_Browser(x+4, y+30, w-8, h-35);
	assert(m_pkBrowser);
	m_pkBrowser->color(FL_BACKGROUND_COLOR);
	m_pkBrowser->textsize(12);
	browser_load();
	m_pkBrowser->callback((Fl_Callback *)browser_callback, this);
	pkBinGroup->resizable(m_pkBrowser);
	pkBinGroup->end();

	// Tabs Group
	Fl_Tabs* pkTabsGroup = new Fl_Tabs(x, y, w, h);
	pkTabsGroup->box(FL_THIN_UP_FRAME);
	pkTabsGroup->add(pkMainGroup);
	pkTabsGroup->add(pkBinGroup);
	pkTabsGroup->resizable(pkMainGroup);
	pkTabsGroup->end();
	
	resizable(pkTabsGroup);
	box(FL_FLAT_BOX);
	color(FL_BACKGROUND_COLOR);
	end();

	// Consumer
	char consumer_id[] = "iola"; //NOTE Using our custom consumer
	Mlt::Profile kProfile; //NOTE The MLT default profile
	m_pkConsumer = Mlt::Factory::consumer(kProfile, consumer_id);
	assert(m_pkConsumer->get_consumer());

	m_pkConsumer->lock();
	m_pkConsumer->set("app_locked", 1);
	m_pkConsumer->set("app_lock", (void *)Fl::lock, 0);
	m_pkConsumer->set("app_unlock", (void *)Fl::unlock, 0);
	m_pkFrameShowEvent = m_pkConsumer->listen("consumer-frame-show", this, 
						  (mlt_listener)frame_show_callback);
	m_pkConsumer->unlock();
	assert(m_pkFrameShowEvent);

	rDebug("%s: Source monitor constructed", __PRETTY_FUNCTION__);
}

SourceMonitor::~SourceMonitor()
{
	assert(m_pkFrameShowEvent);
	delete m_pkFrameShowEvent;

	// Video settings change signals
	on_width_change_connection.disconnect();
	on_height_change_connection.disconnect();
	on_dar_change_connection.disconnect();
	on_par_change_connection.disconnect();
	on_progressive_change_connection.disconnect();
	on_fps_change_connection.disconnect();
	on_timebase_change_connection.disconnect();
	on_ntsc_change_connection.disconnect();

	// Audio settings change signals
	on_sample_depth_change_connection.disconnect();
	on_sample_rate_change_connection.disconnect();

	// Markers change signal
	on_marks_change_connection.disconnect();

	// Source change signals
	on_playback_connection.disconnect();
	on_producer_change_connection.disconnect();

	assert(m_pkConsumer);
	m_pkConsumer->stop();
	delete m_pkConsumer;

	rDebug("%s: Source monitor destructed", __PRETTY_FUNCTION__);
}

void SourceMonitor::connect_to(iola::model::imodel* model)
{
	if (!model)
		throw std::invalid_argument("Model passed as parameter is NULL");

	m_pkSource = model->source();

	assert(m_pkSource);
	assert(m_pkConsumer);
	m_pkSource->connect_to(m_pkConsumer);

	// Video settings change signals
	on_width_change_connection = m_pkSource->on_width_change_signal.connect(
		boost::bind(&SourceMonitor::set_width, this, _1)
		);
	on_height_change_connection = m_pkSource->on_height_change_signal.connect(
		boost::bind(&SourceMonitor::set_height, this, _1)
		);
	on_dar_change_connection = m_pkSource->on_dar_change_signal.connect(
		boost::bind(&SourceMonitor::set_dar, this, _1)
		);
	on_par_change_connection = m_pkSource->on_par_change_signal.connect(
		boost::bind(&SourceMonitor::set_par, this, _1)
		);
	on_fps_change_connection = m_pkSource->on_fps_change_signal.connect(
		boost::bind(&SourceMonitor::set_fps, this, _1)
		);
	on_timebase_change_connection = m_pkSource->on_timebase_change_signal.connect(
		boost::bind(&SourceMonitor::set_timebase, this, _1)
		);
	on_ntsc_change_connection = m_pkSource->on_ntsc_change_signal.connect(
		boost::bind(&SourceMonitor::set_ntsc, this, _1)
		);
	on_progressive_change_connection = m_pkSource->on_progressive_change_signal.connect(
		boost::bind(&SourceMonitor::set_progressive, this, _1)
		);

	// Audio settings change signals
	on_sample_depth_change_connection = m_pkSource->on_sample_depth_change_signal.connect(
		boost::bind(&SourceMonitor::set_sample_depth, this, _1)
		);
	on_sample_rate_change_connection = m_pkSource->on_sample_rate_change_signal.connect(
		boost::bind(&SourceMonitor::set_sample_rate, this, _1)
		);

	// Markers change signal
	on_marks_change_connection = m_pkSource->on_mark_change_signal.connect(
		boost::bind(&SourceMonitor::on_marks_change, this, _1, _2)
		);

	// Source change signals
	on_playback_connection = m_pkSource->on_playback_change_signal.connect(
		boost::bind(&SourceMonitor::on_playback_change, this)
		);
	on_producer_change_connection = m_pkSource->on_producer_change_signal.connect(
		boost::bind(&SourceMonitor::on_producer_change, this, _1, _2)
		);
}

void SourceMonitor::set_width(const int width)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("width", width);
	m_pkConsumer->unlock();
}

void SourceMonitor::set_height(const int height)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("height", height);
	m_pkConsumer->unlock();
}

void SourceMonitor::set_dar(const boost::rational<int> dar)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("display_aspect_num", dar.numerator());
	m_pkConsumer->set("display_aspect_den", dar.denominator());
	m_pkConsumer->unlock();
}

void SourceMonitor::set_par(const boost::rational<int> par)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("sample_aspect_num", par.numerator());
	m_pkConsumer->set("sample_aspect_den", par.denominator());
	m_pkConsumer->unlock();
}

void SourceMonitor::set_fps(const boost::rational<int> fps)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("frame_rate_num", fps.numerator());
	m_pkConsumer->set("frame_rate_den", fps.denominator());
	m_pkConsumer->unlock();
}

void SourceMonitor::set_timebase(const int timebase)
{
	assert(m_pkTimecode);
	m_pkTimecode->set_timebase(timebase);
}

void SourceMonitor::set_ntsc(const bool ntsc)
{
	assert(m_pkTimecode);
	m_pkTimecode->set_dropframe(ntsc);
}

void SourceMonitor::set_progressive(const bool progressive)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("progressive", progressive);
	m_pkConsumer->unlock();
}

void SourceMonitor::set_sample_depth(const int depth)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("audio_format", depth);
	m_pkConsumer->unlock();
}

void SourceMonitor::set_sample_rate(const int rate)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("frequency", rate);
	m_pkConsumer->unlock();
}

void SourceMonitor::on_marks_change(const int in, const int out)
{
	assert(m_pkSlider);
	m_pkSlider->marks(in, out);
}

void SourceMonitor::on_playback_change()
{
	refresh();
}

void SourceMonitor::on_producer_change(const int start, const int end)
{
	assert(m_pkSlider);
	m_pkSlider->bounds(start, end);
}

int SourceMonitor::handle(int event)
{
	switch(event)
	{
	case FL_PUSH:
		rDebug("%s: Recieved mouse click", __PRETTY_FUNCTION__);
		color(FL_DARK1); 
		redraw();
		Fl::focus(this);
		restart();
		return Fl_Group::handle(event);
	case FL_FOCUS:
		rDebug("%s: Got focus", __PRETTY_FUNCTION__);
		restart();
		color(FL_DARK1); 
		redraw();
		return 1;
	case FL_UNFOCUS:
		rDebug("%s: Lost focus", __PRETTY_FUNCTION__);
		stop_playback();
		color(FL_BACKGROUND_COLOR); 
		redraw();
		return 1;
	case FL_KEYUP:
		if (Fl::event_key() == 'q')
		{
			goto_mark_in();
			return 1;
		}
		else if (Fl::event_key() == 'w')
		{
			goto_mark_out();
			return 1;
		}
		else if (Fl::event_key() == 'e')
		{
			mark_in();
			return 1;
		}
		else if (Fl::event_key() == 'r')
		{
			mark_out();
			return 1;
		}
		else if (Fl::event_key() == 't')
		{
			mark_clip();
			return 1;
		}
		else if (Fl::event_key() == 'i')
		{
			mark_in();
			return 1;
		}
		else if (Fl::event_key() == 'o')
		{
			mark_out();
			return 1;
		}
		else if (Fl::event_key() == 'd')
		{
			clear_mark_in();
			return 1;
		}
		else if (Fl::event_key() == 'f')
		{
			clear_mark_out();
			return 1;
		}
		else if (Fl::event_key() == 'g')
		{
			clear_mark_in();
			clear_mark_out();
			return 1;
		}
		else if (Fl::event_key() == 'j')
		{
			if (Fl::event_key('k'))
				step_backward();
			else
				play_backward();
			return 1;
		}
		else if (Fl::event_key() == 'k')
		{
			stop_playback();
			return 1;
		}
		else if (Fl::event_key() == 'l')
		{
			if (Fl::event_key('k'))
				step_forward();
			else
				play_forward();
			return 1;
		}
		else if (Fl::event_key() == FL_Home)
		{
			goto_start();
			return 1;
		}
		else if (Fl::event_key() == FL_End)
		{
			goto_end();
			return 1;
		}
		return 0;
	default:
		return Fl_Group::handle(event);
	}
}

void SourceMonitor::frame_shown(Mlt::Frame &frame)
{
	assert(m_pkConsumer);
	assert(m_pkSlider);
	assert(m_pkTimecode);

	if (!m_pkConsumer->is_stopped())
	{
		//NOTE Do not use Fl::lock() here, it will deadlock when the consumer is stopping!
		int position = frame.get_int("_position");
		m_pkSlider->value(position);
		m_pkTimecode->value(position);
	}
}

void SourceMonitor::browser_load()
{
	assert(m_pkBrowser);
	m_pkBrowser->clear();
	m_pkBrowser->add(".");
	m_pkBrowser->add("..");
	boost::filesystem::path dir_path = boost::filesystem::current_path();
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(dir_path); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_directory(itr->path()) || 
		    boost::filesystem::extension(itr->path()) == ".xml")
			m_pkBrowser->add(itr->path().filename().c_str());
	}
}

void SourceMonitor::browser_callback()
{
	assert(m_pkBrowser);
	int selected = m_pkBrowser->value();
	if (selected && Fl::event_clicks())
	{
		Fl::event_clicks(0);
		boost::filesystem::path selected_path(m_pkBrowser->text(selected));
		if (boost::filesystem::is_directory(selected_path))
		{
			boost::filesystem::current_path(selected_path);
			browser_load();
		}
		else
		{
			m_pkSource->load_file(
				boost::filesystem::current_path() / selected_path
				);
		}
	}
}

void SourceMonitor::slider_callback()
{
	if (!m_pkSource)
		return;
	m_pkSource->goto_frame(m_pkSlider->value());
}

void SourceMonitor::mark_in()
{
	if (!m_pkSource)
		return;
	m_pkSource->mark_in();
}

void SourceMonitor::mark_out()
{
	if (!m_pkSource)
		return;
	m_pkSource->mark_out();
}

void SourceMonitor::mark_clip()
{
	if (!m_pkSource)
		return;
	m_pkSource->mark_clip();
}

void SourceMonitor::clear_mark_in()
{
	if (!m_pkSource)
		return;
	m_pkSource->clear_mark_in();
}

void SourceMonitor::clear_mark_out()
{
	if (!m_pkSource)
		return;
	m_pkSource->clear_mark_out();
}

void SourceMonitor::goto_mark_in()
{
	if (!m_pkSource)
		return;
	m_pkSource->goto_mark_in();
}

void SourceMonitor::goto_mark_out()
{
	if (!m_pkSource)
		return;
	m_pkSource->goto_mark_out();
}

void SourceMonitor::goto_start()
{
	if (!m_pkSource)
		return;
	m_pkSource->goto_start();
}

void SourceMonitor::goto_end()
{
	if (!m_pkSource)
		return;
	m_pkSource->goto_end();
}

void SourceMonitor::play_backward()
{
	if (!m_pkSource)
		return;
	m_pkSource->play_backward();
}

void SourceMonitor::play_forward()
{
	if (!m_pkSource)
		return;
	m_pkSource->play_forward();
}

void SourceMonitor::step_backward()
{
	if (!m_pkSource)
		return;
	m_pkSource->step_backward();
}

void SourceMonitor::step_forward()
{
	if (!m_pkSource)
		return;
	m_pkSource->step_forward();
}

void SourceMonitor::stop_playback()
{
	if (!m_pkSource)
		return;
	m_pkSource->stop();
}

Window SourceMonitor::xid()
{
	assert(m_pkDisplay);
	return m_pkDisplay->shown() ? fl_xid(m_pkDisplay) : 0;
}

void SourceMonitor::stop()
{
	assert(m_pkConsumer);
	rDebug("%s: Stopping consumer", __PRETTY_FUNCTION__);
	m_pkConsumer->stop();
}

bool SourceMonitor::restart()
{
	assert(m_pkConsumer);
	bool ret = m_pkConsumer->is_stopped() && xid() != 0;
	if (ret)
	{
		rDebug("%s: Starting consumer with xid=%i", __PRETTY_FUNCTION__, (int)xid());
		m_pkConsumer->set("xid", (int)xid());
		m_pkConsumer->start();
	}
	if (!m_pkConsumer->is_stopped())
		refresh();
	return ret;
}

void SourceMonitor::refresh()
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}

} // namespace gui
} // namespace iola
