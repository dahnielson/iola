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
#include <cstdlib>
#include <iostream>

// IOLA
#include "ProgramMonitor.h"

ProgramMonitor::ProgramMonitor(MainWindow* parent, int x, int y, int w, int h, const char *label) :
	Fl_Group(x, y, w, h, label),
	m_pkParent(parent),
	m_pkConsumer(0),
	m_pkFrameShowEvent(0),
	m_pkProducerChangedEvent(0),
	m_pkSlider(0)
{
	// Transport Slider
	m_pkSlider = new Fl_Slider(x+4, y+h-50, w-8, 19);
	m_pkSlider->type(FL_HOR_SLIDER);
	m_pkSlider->slider_size(0.02);
	m_pkSlider->bounds(0, 100);
	m_pkSlider->precision(0);
	m_pkSlider->callback((Fl_Callback *)slider_callback, this);
	
	// Transport Buttons
	Fl_Button *pkMarkIn = new Fl_Button(0, 0, 25, 25, "[");
	pkMarkIn->type(FL_NORMAL_BUTTON);
	pkMarkIn->callback((Fl_Callback *)mark_in, this);

	Fl_Button *pkPlayBackward = new Fl_Button(0, 0, 25, 25, "@<|");
	pkPlayBackward->type(FL_NORMAL_BUTTON);
	pkPlayBackward->callback((Fl_Callback *)play_backward, this);

	Fl_Button *pkStop = new Fl_Button(0, 0, 25, 25, "@||");
	pkStop->type(FL_NORMAL_BUTTON);
	pkStop->callback((Fl_Callback *)stop_playback, this);

	Fl_Button *pkPlayForward = new Fl_Button(0, 0, 25, 25, "@|>");
	pkPlayForward->type(FL_NORMAL_BUTTON);
	pkPlayForward->callback((Fl_Callback *)play_forward, this);

	Fl_Button *pkMarkOut = new Fl_Button(0, 0, 25, 25, "]");
	pkMarkOut->type(FL_NORMAL_BUTTON);
	pkMarkOut->callback((Fl_Callback *)mark_out, this);
	
	// Transport Button Group
	Fl_Pack *pkTransportGroup = new Fl_Pack(x+w/2-62, y+h-28, 125, 25);
	pkTransportGroup->box(FL_NO_BOX);
	pkTransportGroup->type(Fl_Pack::HORIZONTAL);
	pkTransportGroup->add(pkMarkIn);
	pkTransportGroup->add(pkPlayBackward);
	pkTransportGroup->add(pkStop);
	pkTransportGroup->add(pkPlayForward);
	pkTransportGroup->add(pkMarkOut);
	pkTransportGroup->end();

	// Main Group
	Fl_Group *pkMainGroup = new Fl_Group(x, y+20, w, h-25, "Program");
	pkMainGroup->labelsize(11);
	m_pkDisplay = new Fl_Single_Window(x+4, y+30, w-8, h-82);
	m_pkDisplay->color(FL_BLACK);
	m_pkDisplay->box(FL_FLAT_BOX);
	m_pkDisplay->end();
	pkMainGroup->add(m_pkDisplay);
	pkMainGroup->add(m_pkSlider);
	pkMainGroup->add(pkTransportGroup);
	pkMainGroup->resizable(m_pkDisplay);
	pkMainGroup->end();
	
	// Tabs Group
	Fl_Tabs* pkTabsGroup = new Fl_Tabs(x, y, w, h);
	pkTabsGroup->box(FL_THIN_UP_FRAME);
	pkTabsGroup->add(pkMainGroup);
	pkTabsGroup->resizable(pkMainGroup);
	pkTabsGroup->end();
	
	resizable(pkTabsGroup);
	box(FL_FLAT_BOX);
	color(FL_BACKGROUND_COLOR);
	end();

	// Consumer
	m_pkConsumer = new Mlt::Consumer(m_pkParent->get_profile(), "sdl");
	m_pkConsumer->set("app_locked", 1);
	m_pkConsumer->set("app_lock", (void *)Fl::lock, 0);
	m_pkConsumer->set("app_unlock", (void *)Fl::unlock, 0);
	m_pkFrameShowEvent = m_pkConsumer->listen("consumer-frame-show", this, (mlt_listener)frame_show_callback);

	// Connect signals
	on_program_load_connection = m_pkParent->on_program_load_signal.connect(
		boost::bind(&ProgramMonitor::on_program_load, this)
		);
	on_program_playback_connection = m_pkParent->on_program_playback_signal.connect(
		boost::bind(&ProgramMonitor::on_program_playback, this)
		);
}

ProgramMonitor::~ProgramMonitor()
{
	delete m_pkFrameShowEvent;
	delete m_pkProducerChangedEvent;
	on_program_load_connection.disconnect();
	on_program_playback_connection.disconnect();
	if (m_pkConsumer)
		m_pkConsumer->stop();
	delete m_pkConsumer;
}

int ProgramMonitor::handle(int event)
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
		stop();
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
			mark_cut();
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
		else if (Fl::event_key() == 'a')
		{
			edit_previous_goto();
			return 1;
		}
		else if (Fl::event_key() == 's')
		{
			edit_next_goto();
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
		else if (Fl::event_key() == 'v')
		{
			insert();
			return 1;
		}
		else if (Fl::event_key() == 'b')
		{
			overwrite();
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

void ProgramMonitor::on_program_load()
{
	rDebug("%s: Connect program to consumer", __PRETTY_FUNCTION__);
	m_pkConsumer->lock();
	m_pkConsumer->connect(m_pkParent->get_program());
	m_pkConsumer->unlock();
	m_pkProducerChangedEvent = m_pkParent->get_program().listen("producer-changed", this, (mlt_listener)producer_changed_callback);
}

void ProgramMonitor::on_program_playback()
{
	refresh();
}

void ProgramMonitor::frame_shown(Mlt::Frame &frame)
{
	if (m_pkConsumer && !m_pkConsumer->is_stopped())
	{
		Fl::lock();
		m_pkSlider->value(frame.get_int("_position"));
		Fl::check();
		Fl::unlock();
	}
}

void ProgramMonitor::producer_changed()
{
	Fl::lock();
	m_pkSlider->bounds(m_pkParent->get_program().get_in(), m_pkParent->get_program().get_out());
	Fl::unlock();
}

void ProgramMonitor::slider_callback()
{
	Fl::lock();
	m_pkParent->program_seek(m_pkSlider->value());
	Fl::unlock();
}

void ProgramMonitor::mark_in()
{
	if (m_pkParent)
		m_pkParent->program_set_mark_in();
}

void ProgramMonitor::mark_out()
{
	if (m_pkParent)
		m_pkParent->program_set_mark_out();
}

void ProgramMonitor::mark_cut()
{
	if (m_pkParent)
		m_pkParent->program_set_mark_cut();
}

void ProgramMonitor::mark_in_clear()
{
	if (m_pkParent)
		m_pkParent->program_clear_mark_in();
}

void ProgramMonitor::mark_out_clear()
{
	if (m_pkParent)
		m_pkParent->program_clear_mark_out();
}

void ProgramMonitor::mark_in_goto()
{
	if (m_pkParent)
		m_pkParent->program_goto_mark_in();
}

void ProgramMonitor::mark_out_goto()
{
	if (m_pkParent)
		m_pkParent->program_goto_mark_out();
}

void ProgramMonitor::goto_start()
{
	if (m_pkParent)
		m_pkParent->program_goto_start();
}

void ProgramMonitor::goto_end()
{
	if (m_pkParent)
		m_pkParent->program_goto_end();
}

void ProgramMonitor::step_backward()
{
	if (m_pkParent)
		m_pkParent->program_step_backward();
}

void ProgramMonitor::step_forward()
{
	if (m_pkParent)
		m_pkParent->program_step_forward();
}

void ProgramMonitor::play_backward()
{
	if (m_pkParent)
		m_pkParent->program_play_reverse();
}

void ProgramMonitor::play_forward()
{
	if (m_pkParent)
		m_pkParent->program_play_forward();
}

void ProgramMonitor::stop_playback()
{
	if (m_pkParent)
		m_pkParent->program_pause();
}

void ProgramMonitor::edit_previous_goto()
{
	if (m_pkParent)
		m_pkParent->program_goto_previous_edit();
}

void ProgramMonitor::edit_next_goto()
{
	if (m_pkParent)
		m_pkParent->program_goto_next_edit();
}

void ProgramMonitor::insert()
{
	if (m_pkParent)
		m_pkParent->program_insert();
}

void ProgramMonitor::overwrite()
{
	if (m_pkParent)
		m_pkParent->program_overwrite();
}

Window ProgramMonitor::xid()
{
	return m_pkDisplay->shown() ? fl_xid(m_pkDisplay) : 0;
}

void ProgramMonitor::stop()
{
	if (m_pkConsumer)
	{
		rDebug("%s: Stop consumer", __PRETTY_FUNCTION__);
		m_pkConsumer->stop();
	}
}

bool ProgramMonitor::restart()
{
	bool ret = m_pkConsumer->is_stopped() && xid() != 0;
	if (ret)
	{
		char temp[132];
		sprintf(temp, "%d", (int)xid());
		setenv("SDL_WINDOWID", temp, 1);
		rDebug("%s: Start consumer with xid=%i", __PRETTY_FUNCTION__, (int)xid());
		m_pkConsumer->start();
	}
	if (!m_pkConsumer->is_stopped())
		refresh();
	return ret;
}

void ProgramMonitor::refresh()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}
