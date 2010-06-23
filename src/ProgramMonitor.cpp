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
#include <cstdlib>
#include <iostream>

// IOLA
#include "ProgramMonitor.h"

ProgramMonitor::ProgramMonitor(MainWindow* parent, int x, int y, int w, int h, const char *label) :
	Fl_Group(x, y, w, h, label),
	m_pkParent(parent),
	m_pkConsumer(0)
{
	// Transport Slider
	Fl_Slider *pkSlider = new Fl_Slider(x+4, y+h-50, w-8, 19);
	pkSlider->type(FL_HOR_SLIDER);
//	pkSlider->color(FL_BACKGROUND_COLOR);
	
	// Transport Buttons
	Fl_Button *pkMarkIn = new Fl_Button(0, 0, 25, 25, "[");
	pkMarkIn->type(FL_NORMAL_BUTTON);
	pkMarkIn->shortcut('i');
	pkMarkIn->callback((Fl_Callback *)mark_in, this);

	Fl_Button *pkPlayBackward = new Fl_Button(0, 0, 25, 25, "@<|");
	pkPlayBackward->type(FL_NORMAL_BUTTON);
	pkPlayBackward->shortcut('j');
	pkPlayBackward->callback((Fl_Callback *)play_backward, this);

	Fl_Button *pkStop = new Fl_Button(0, 0, 25, 25, "@||");
	pkStop->type(FL_NORMAL_BUTTON);
	pkStop->shortcut('k');
	pkStop->callback((Fl_Callback *)stop, this);

	Fl_Button *pkPlayForward = new Fl_Button(0, 0, 25, 25, "@|>");
	pkPlayForward->type(FL_NORMAL_BUTTON);
	pkPlayForward->shortcut('l');
	pkPlayForward->callback((Fl_Callback *)play_forward, this);

	Fl_Button *pkMarkOut = new Fl_Button(0, 0, 25, 25, "]");
	pkMarkOut->type(FL_NORMAL_BUTTON);
	pkMarkOut->shortcut('o');
	pkMarkOut->callback((Fl_Callback *)mark_out, this);
	
	// Transport Button Group
	Fl_Pack *pkTransportGroup = new Fl_Pack(x+w/2-62, y+h-28, 125, 25);
	pkTransportGroup->box(FL_NO_BOX);
	pkTransportGroup->type(Fl_Pack::HORIZONTAL);
//	pkTransportGroup->spacing(2);
	pkTransportGroup->add(pkMarkIn);
	pkTransportGroup->add(pkPlayBackward);
	pkTransportGroup->add(pkStop);
	pkTransportGroup->add(pkPlayForward);
	pkTransportGroup->add(pkMarkOut);
	pkTransportGroup->end();

	// Main Group
	Fl_Group *pkMainGroup = new Fl_Group(x, y+20, w, h-25, "Program");
	pkMainGroup->labelsize(11);
	pkMainGroup->color(FL_DARK1);
	m_pkDisplay = new Fl_Window(x+4, y+30, w-8, h-82);
	m_pkDisplay->color(FL_BLACK);
	m_pkDisplay->box(FL_FLAT_BOX);
	pkMainGroup->add(m_pkDisplay);
	pkMainGroup->add(pkSlider);
	pkMainGroup->add(pkTransportGroup);
	pkMainGroup->resizable(m_pkDisplay);
	pkMainGroup->end();
	
	// Tabs Group
	Fl_Tabs* pkTabsGroup = new Fl_Tabs(x, y, w, h);
	pkTabsGroup->add(pkMainGroup);
	pkTabsGroup->resizable(pkMainGroup);
	pkTabsGroup->end();
	
	resizable(pkTabsGroup);
	box(FL_FLAT_BOX);
	end();

	// Consumer
	m_pkConsumer = new Mlt::Consumer(m_pkParent->get_profile(), "sdl");
	m_pkConsumer->set("app_locked", 1);
	m_pkConsumer->set("app_lock", (void *)Fl::lock, 0);
	m_pkConsumer->set("app_unlock", (void *)Fl::unlock, 0);

	// Connect signals
	on_program_load_connection = m_pkParent->on_program_load_signal.connect(
		boost::bind(&ProgramMonitor::on_program_load, this)
		);
	on_program_playback_connection = m_pkParent->on_program_playback_signal.connect(
		boost::bind(&ProgramMonitor::on_program_playback, this)
		);

	restart();
}

ProgramMonitor::~ProgramMonitor()
{
	on_program_load_connection.disconnect();
	on_program_playback_connection.disconnect();
	if (m_pkConsumer)
		m_pkConsumer->stop();
	delete m_pkConsumer;
}

void ProgramMonitor::on_program_load()
{
	m_pkConsumer->lock();
	m_pkConsumer->connect(m_pkParent->get_program());
	m_pkConsumer->unlock();
	restart();
}

void ProgramMonitor::on_program_playback()
{
	refresh();
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

void ProgramMonitor::stop()
{
	if (m_pkParent)
		m_pkParent->program_pause();
}

Window ProgramMonitor::xid()
{
	return m_pkDisplay->shown() ? fl_xid(m_pkDisplay) : 0;
}

bool ProgramMonitor::restart()
{
	bool ret = m_pkConsumer->is_stopped() && xid() != 0;
	if (ret)
	{
		char temp[132];
		sprintf(temp, "%d", (int)xid());
		setenv("SDL_WINDOWID", temp, 1);
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
