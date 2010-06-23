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

// BOOST
#include <boost/filesystem.hpp>

// IOLA
#include "SourceMonitor.h"

SourceMonitor::SourceMonitor(MainWindow* parent, int x, int y, int w, int h, const char *label) :
	Fl_Group(x, y, w, h, label),
	m_pkParent(parent),
	m_pkConsumer(0)
{
	// Transport Slider
	Fl_Slider *pkSlider = new Fl_Slider(x+4, y+h-50, w-8, 19);
	pkSlider->type(FL_HOR_SLIDER);
	pkSlider->labelcolor(FL_FOREGROUND_COLOR);
	pkSlider->color(FL_BLUE);
	
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
	Fl_Group *pkMainGroup = new Fl_Group(x, y+20, w, h-25, "Source");
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
	pkTabsGroup->add(pkMainGroup);
	pkTabsGroup->add(pkBinGroup);
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
	on_source_load_connection = m_pkParent->on_source_load_signal.connect(
		boost::bind(&SourceMonitor::on_source_load, this)
		);
	on_source_playback_connection = m_pkParent->on_source_playback_signal.connect(
		boost::bind(&SourceMonitor::on_source_playback, this)
		);

	restart();
}

SourceMonitor::~SourceMonitor()
{
	on_source_load_connection.disconnect();
	on_source_playback_connection.disconnect();
	if (m_pkConsumer)
		m_pkConsumer->stop();
	delete m_pkConsumer;
}

void SourceMonitor::on_source_load()
{
	m_pkConsumer->lock();
	m_pkConsumer->connect(m_pkParent->get_source());
	m_pkConsumer->unlock();
	restart();
}

void SourceMonitor::on_source_playback()
{
	refresh();
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
	if (selected)
	{
		boost::filesystem::path selected_path(m_pkBrowser->text(selected));
		if (boost::filesystem::is_directory(selected_path))
		{
			chdir(selected_path.filename().c_str());
			browser_load();
		}
		else if(Fl::event_clicks())
		{
			m_pkParent->source_load(selected_path.filename().c_str());
		}
	}
}

void SourceMonitor::mark_in()
{
	if (m_pkParent)
		m_pkParent->source_set_mark_in();
}

void SourceMonitor::mark_out()
{
	if (m_pkParent)
		m_pkParent->source_set_mark_out();
}

void SourceMonitor::mark_in_clear()
{
	if (m_pkParent)
		m_pkParent->source_clear_mark_in();
}

void SourceMonitor::mark_out_clear()
{
	if (m_pkParent)
		m_pkParent->source_clear_mark_out();
}

void SourceMonitor::play_backward()
{
	if (m_pkParent)
		m_pkParent->source_play_reverse();
}

void SourceMonitor::play_forward()
{
	if (m_pkParent)
		m_pkParent->source_play_forward();
}

void SourceMonitor::stop()
{
	if (m_pkParent)
		m_pkParent->source_pause();
}

Window SourceMonitor::xid()
{
	return m_pkDisplay->shown() ? fl_xid(m_pkDisplay) : 0;
}

bool SourceMonitor::restart()
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

void SourceMonitor::refresh()
{
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}
