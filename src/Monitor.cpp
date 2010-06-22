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
#include "Monitor.h"

Monitor::Monitor(MainWindow* parent, int x, int y, int w, int h, const char *label) :
	Fl_Group(x, y, w, h, label),
	m_pkParent(parent),
	m_pkConsumer(0),
	m_pkProducer(0)
{
	pthread_mutex_init(&mutex, NULL);

	// Transport Slider
	Fl_Slider *pkSlider = new Fl_Slider(0, 0, w, 20);
	pkSlider->type(FL_HOR_SLIDER);
	pkSlider->labelcolor(FL_FOREGROUND_COLOR);
//	pkSlider->color(FL_BACKGROUND_COLOR);
	
	// Transport Buttons
	Fl_Button *pkMarkIn = new Fl_Button(0, 0, 25, 25, "[");
	pkMarkIn->type(FL_NORMAL_BUTTON);
	pkMarkIn->labelcolor(FL_BLACK);
//	pkMarkIn->color(FL_BACKGROUND_COLOR);
	pkMarkIn->shortcut('i');
	pkMarkIn->callback((Fl_Callback *)mark_in, this);

	Fl_Button *pkPlayBackward = new Fl_Button(0, 0, 25, 25, "@<|");
	pkPlayBackward->type(FL_NORMAL_BUTTON);
	pkPlayBackward->labelcolor(FL_BLACK);
//	pkPlayBackward->color(FL_BACKGROUND_COLOR);
	pkPlayBackward->shortcut('j');
	pkPlayBackward->callback((Fl_Callback *)play_backward, this);

	Fl_Button *pkStop = new Fl_Button(0, 0, 25, 25, "@||");
	pkStop->type(FL_NORMAL_BUTTON);
	pkStop->labelcolor(FL_BLACK);
//	pkStop->color(FL_BACKGROUND_COLOR);
	pkStop->shortcut('k');
	pkStop->callback((Fl_Callback *)stop, this);

	Fl_Button *pkPlayForward = new Fl_Button(0, 0, 25, 25, "@|>");
	pkPlayForward->type(FL_NORMAL_BUTTON);
	pkPlayForward->labelcolor(FL_BLACK);
//	pkPlayForward->color(FL_BACKGROUND_COLOR);
	pkPlayForward->shortcut('l');
	pkPlayForward->callback((Fl_Callback *)play_forward, this);

	Fl_Button *pkMarkOut = new Fl_Button(0, 0, 25, 25, "]");
	pkMarkOut->type(FL_NORMAL_BUTTON);
	pkMarkOut->labelcolor(FL_BLACK);
//	pkMarkOut->color(FL_BACKGROUND_COLOR);
	pkMarkOut->shortcut('o');
	pkMarkOut->callback((Fl_Callback *)mark_out, this);
	
	// Transport Button Group
	Fl_Pack *pkTransportGroup = new Fl_Pack(0, 0, w, 25);
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
	Fl_Pack *pkMainGroup = new Fl_Pack(x+2, y+2, w-4, h-4);
	m_pkDisplay = new Fl_Window(x, y, w, h-45-4);
	m_pkDisplay->color(FL_BLACK);
	m_pkDisplay->box(FL_FLAT_BOX);
	pkMainGroup->add(m_pkDisplay);
	pkMainGroup->add(pkSlider);
	pkMainGroup->add(pkTransportGroup);
	pkMainGroup->resizable(m_pkDisplay);
	pkMainGroup->end();
	
	resizable(pkMainGroup);
	box(FL_THIN_UP_BOX);
	end();

	m_pkConsumer = new Mlt::Consumer(m_pkParent->get_profile(), "sdl");
	m_pkConsumer->set("app_locked", 1);
	m_pkConsumer->set("app_lock", (void *)Fl::lock, 0);
	m_pkConsumer->set("app_unlock", (void *)Fl::unlock, 0);

	restart();
}

Monitor::~Monitor()
{
	if (m_pkConsumer)
		m_pkConsumer->stop();
	delete m_pkConsumer;
	delete m_pkProducer;
	pthread_mutex_destroy(&mutex);
}

void Monitor::load(Mlt::Producer &producer)
{
	pthread_mutex_lock(&mutex);
	m_pkProducer = new Mlt::Producer(producer);
	m_pkProducer->set_speed(0);
	m_pkConsumer->lock();
	m_pkConsumer->connect(*m_pkProducer);
	m_pkConsumer->unlock();
	restart();
	pthread_mutex_unlock(&mutex);
}

void Monitor::mark_in()
{
	if (m_pkProducer)
		m_pkProducer->set("meta.iola.mark_in", m_pkProducer->frame());
}

void Monitor::mark_out()
{
	if (m_pkProducer)
		m_pkProducer->set("meta.iola.mark_out", m_pkProducer->frame());
}

void Monitor::mark_in_clear()
{
	if (m_pkProducer)
		m_pkProducer->set("meta.iola.mark_in", -1);
}

void Monitor::mark_out_clear()
{
	if (m_pkProducer)
		m_pkProducer->set("meta.iola.mark_out", -1);
}

void Monitor::play_backward()
{
	set_speed(-1);
}

void Monitor::play_forward()
{
	set_speed(1);
}

void Monitor::stop()
{
	set_speed(0);
}

Window Monitor::xid()
{
	return m_pkDisplay->shown() && m_pkDisplay->visible() ? fl_xid(m_pkDisplay) : 0;
}

bool Monitor::restart()
{
	bool ret = m_pkConsumer->is_stopped() && m_pkDisplay->shown() && xid() != 0;
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

void Monitor::refresh()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}

void Monitor::set_speed(double speed)
{
	pthread_mutex_lock(&mutex);
	if (m_pkProducer && m_pkProducer->get_speed() != speed)
	{
		m_pkProducer->lock();
//		position += 2 * ( int )m_pkProducer->get_speed();
//		m_pkProducer->seek(position);
		m_pkProducer->set_speed(speed);
//		speed = m_pkProducer->get_speed();
		m_pkProducer->unlock();
	}

	refresh();
	pthread_mutex_unlock(&mutex);
}

void Monitor::seek(int pos)
{
	if (m_pkProducer)
	{
		m_pkProducer->lock();
		m_pkProducer->seek(pos < 0 ? 0 : pos > m_pkProducer->get_out() ? m_pkProducer->get_out() : pos);
		m_pkProducer->unlock();
	}
	refresh();
}

