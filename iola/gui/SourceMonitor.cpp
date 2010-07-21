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

// RLOG
#include <rlog/rlog.h>

// STD
#include <iostream>
#include <exception>

// BOOST
#include <boost/filesystem.hpp>

// IOLA
#include <iola/application/get_instance.h>
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

SourceMonitor::SourceMonitor(int x, int y, int w, int h, const char *label) :
	Fl_Group(x, y, w, h, label),
	m_pkConsumer(0),
	m_pkFrameShowEvent(0),
	m_pkSlider(0),
	m_pkTimecode(0)
{
	// Transport Slider
	m_pkSlider = new TimeRuler(x+4, y+h-50, w-8, 19);
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

	pkTransportGroup->end();

	// Main Group
	Fl_Group *pkMainGroup = new Fl_Group(x, y+20, w, h-25, "Source");
	pkMainGroup->labelsize(11);
	m_pkDisplay = new Fl_Window(x+4, y+30, w-8, h-82);
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
	m_pkConsumer = Mlt::Factory::consumer(iola::application::get_instance()->get_project()->get_profile(), "iola");
	if (!m_pkConsumer->get_consumer())
	{
		rError("%s: No consumer!", __PRETTY_FUNCTION__);
		throw std::exception();
	}
	m_pkConsumer->lock();
	m_pkConsumer->set("app_locked", 1);
	m_pkConsumer->set("app_lock", (void *)Fl::lock, 0);
	m_pkConsumer->set("app_unlock", (void *)Fl::unlock, 0);
	m_pkFrameShowEvent = m_pkConsumer->listen("consumer-frame-show", this, (mlt_listener)frame_show_callback);
	m_pkConsumer->unlock();

	iola::application::get_instance()->get_project()->source_connect_consumer(m_pkConsumer);

	// Connect signals
	on_sar_change_connection = iola::application::get_instance()->get_project()->on_sar_change_signal.connect(
		boost::bind(&SourceMonitor::on_sar_change, this)
		);
	on_dar_change_connection = iola::application::get_instance()->get_project()->on_dar_change_signal.connect(
		boost::bind(&SourceMonitor::on_dar_change, this)
		);
	on_par_change_connection = iola::application::get_instance()->get_project()->on_par_change_signal.connect(
		boost::bind(&SourceMonitor::on_par_change, this)
		);
	on_field_change_connection = iola::application::get_instance()->get_project()->on_field_change_signal.connect(
		boost::bind(&SourceMonitor::on_field_change, this)
		);
	on_fps_change_connection = iola::application::get_instance()->get_project()->on_fps_change_signal.connect(
		boost::bind(&SourceMonitor::on_fps_change, this)
		);
	on_sample_change_connection = iola::application::get_instance()->get_project()->on_sample_change_signal.connect(
		boost::bind(&SourceMonitor::on_sample_change, this)
		);
	on_source_load_connection = iola::application::get_instance()->get_project()->on_source_load_signal.connect(
		boost::bind(&SourceMonitor::on_source_load, this)
		);
	on_source_playback_connection = iola::application::get_instance()->get_project()->on_source_playback_signal.connect(
		boost::bind(&SourceMonitor::on_source_playback, this)
		);
	on_source_marks_change_connection = iola::application::get_instance()->get_project()->on_source_marks_change_signal.connect(
		boost::bind(&SourceMonitor::on_source_marks_change, this)
		);
	on_source_producer_change_connection = iola::application::get_instance()->get_project()->on_source_producer_change_signal.connect(
		boost::bind(&SourceMonitor::on_source_producer_change, this)
		);

	rDebug("%s: Source monitor initiated", __PRETTY_FUNCTION__);
}

SourceMonitor::~SourceMonitor()
{
	delete m_pkFrameShowEvent;
	on_sar_change_connection.disconnect();
	on_dar_change_connection.disconnect();
	on_par_change_connection.disconnect();
	on_field_change_connection.disconnect();
	on_fps_change_connection.disconnect();
	on_sample_change_connection.disconnect();
	on_source_load_connection.disconnect();
	on_source_playback_connection.disconnect();
	on_source_marks_change_connection.disconnect();
	on_source_producer_change_connection.disconnect();
	if (m_pkConsumer)
		m_pkConsumer->stop();
	delete m_pkConsumer;
	rDebug("%s: Source monitor demolished", __PRETTY_FUNCTION__);
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
			mark_in_goto();
			return 1;
		}
		else if (Fl::event_key() == 'w')
		{
			mark_out_goto();
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
			mark_in_clear();
			return 1;
		}
		else if (Fl::event_key() == 'f')
		{
			mark_out_clear();
			return 1;
		}
		else if (Fl::event_key() == 'g')
		{
			mark_in_clear();
			mark_out_clear();
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

void SourceMonitor::on_sar_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("width", iola::application::get_instance()->get_project()->get_width());
	m_pkConsumer->set("height", iola::application::get_instance()->get_project()->get_height());
	m_pkConsumer->unlock();
}

void SourceMonitor::on_dar_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("display_aspect_num", iola::application::get_instance()->get_project()->dar().numerator());
	m_pkConsumer->set("display_aspect_den", iola::application::get_instance()->get_project()->dar().denominator());
	m_pkConsumer->unlock();
}

void SourceMonitor::on_par_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("sample_aspect_num", iola::application::get_instance()->get_project()->par().numerator());
	m_pkConsumer->set("sample_aspect_den", iola::application::get_instance()->get_project()->par().denominator());
	m_pkConsumer->unlock();
}

void SourceMonitor::on_field_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("progressive", iola::application::get_instance()->get_project()->get_progressive());
	m_pkConsumer->unlock();
}

void SourceMonitor::on_fps_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("frame_rate_num", iola::application::get_instance()->get_project()->fps().numerator());
	m_pkConsumer->set("frame_rate_den", iola::application::get_instance()->get_project()->fps().denominator());
	m_pkConsumer->unlock();

	m_pkTimecode->set_timebase(iola::application::get_instance()->get_project()->get_fps_timebase());
	m_pkTimecode->set_dropframe(iola::application::get_instance()->get_project()->get_fps_ntsc());
}

void SourceMonitor::on_sample_change()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("audio_format", iola::application::get_instance()->get_project()->get_sample_depth());
	m_pkConsumer->set("frequency", iola::application::get_instance()->get_project()->get_sample_rate());
	m_pkConsumer->unlock();
}

void SourceMonitor::on_source_load()
{
	rDebug("%s: Got source load", __PRETTY_FUNCTION__);
}

void SourceMonitor::on_source_playback()
{
	rDebug("%s: Got source playback", __PRETTY_FUNCTION__);
	refresh();
}

void SourceMonitor::on_source_marks_change()
{
	rDebug("%s: Got source marks change", __PRETTY_FUNCTION__);
	m_pkSlider->marks(
		iola::application::get_instance()->get_project()->source_get_mark_in(),
		iola::application::get_instance()->get_project()->source_get_mark_out()
		);
}

void SourceMonitor::frame_shown(Mlt::Frame &frame)
{
	if (m_pkConsumer && !m_pkConsumer->is_stopped() && m_pkSlider && m_pkTimecode)
	{
		//NOTE Do not use Fl::lock() here, it will deadlock when the consumer is stopping!
		int position = frame.get_int("_position");
		m_pkSlider->value(position);
		m_pkTimecode->value(position);
	}
}

void SourceMonitor::on_source_producer_change()
{
	rDebug("%s: Got producer change", __PRETTY_FUNCTION__);
	m_pkSlider->bounds(
		iola::application::get_instance()->get_project()->source_get_start(),
		iola::application::get_instance()->get_project()->source_get_end()
		);
}

void SourceMonitor::slider_callback()
{
	iola::application::get_instance()->get_project()->source_seek(m_pkSlider->value());
}

void SourceMonitor::browser_load()
{
	m_pkBrowser->clear();
	m_pkBrowser->add(".");
	m_pkBrowser->add("..");
	boost::filesystem::path dir_path = boost::filesystem::current_path();
	boost::filesystem::directory_iterator end_itr;
	for (boost::filesystem::directory_iterator itr(dir_path); itr != end_itr; ++itr)
	{
		if (boost::filesystem::is_directory(itr->path()) || 
		    boost::filesystem::extension(itr->path()) == ".avi")
			m_pkBrowser->add(itr->path().filename().c_str());
	}
}

void SourceMonitor::browser_callback()
{
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
			iola::application::get_instance()->get_project()->source_load(
				boost::filesystem::current_path() / selected_path
				);
		}
	}
}

void SourceMonitor::mark_in()
{
	iola::application::get_instance()->get_project()->source_set_mark_in();
}

void SourceMonitor::mark_out()
{
	iola::application::get_instance()->get_project()->source_set_mark_out();
}

void SourceMonitor::mark_clip()
{
	iola::application::get_instance()->get_project()->source_set_mark_clip();
}

void SourceMonitor::mark_in_clear()
{
	iola::application::get_instance()->get_project()->source_clear_mark_in();
}

void SourceMonitor::mark_out_clear()
{
	iola::application::get_instance()->get_project()->source_clear_mark_out();
}

void SourceMonitor::mark_in_goto()
{
	iola::application::get_instance()->get_project()->source_goto_mark_in();
}

void SourceMonitor::mark_out_goto()
{
	iola::application::get_instance()->get_project()->source_goto_mark_out();
}

void SourceMonitor::goto_start()
{
	iola::application::get_instance()->get_project()->source_goto_start();
}

void SourceMonitor::goto_end()
{
	iola::application::get_instance()->get_project()->source_goto_end();
}

void SourceMonitor::step_backward()
{
	iola::application::get_instance()->get_project()->source_step_backward();
}

void SourceMonitor::step_forward()
{
	iola::application::get_instance()->get_project()->source_step_forward();
}

void SourceMonitor::play_backward()
{
	iola::application::get_instance()->get_project()->source_play_reverse();
}

void SourceMonitor::play_forward()
{
	iola::application::get_instance()->get_project()->source_play_forward();
}

void SourceMonitor::stop_playback()
{
	iola::application::get_instance()->get_project()->source_pause();
}

Window SourceMonitor::xid()
{
	return m_pkDisplay->shown() ? fl_xid(m_pkDisplay) : 0;
}

void SourceMonitor::stop()
{
	if (m_pkConsumer)
	{
		rDebug("%s: Stop consumer", __PRETTY_FUNCTION__);
		m_pkConsumer->stop();
	}
}

bool SourceMonitor::restart()
{
	bool ret = m_pkConsumer->is_stopped() && xid() != 0;
	if (ret)
	{
		char temp[132];
		sprintf(temp, "%d", (int)xid());
		rDebug("%s: Start consumer with xid=%i", __PRETTY_FUNCTION__, (int)xid());
		m_pkConsumer->set("xid", (int)xid());
		m_pkConsumer->start();
	}
	if (!m_pkConsumer->is_stopped())
		refresh();
	return ret;
}

void SourceMonitor::refresh()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}

} // namespace gui
} // namespace iola
