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

// IOLA
#include <iola/application/get_instance.h>

#include <iola/application/iapplication.h>
#include <iola/model/imodel.h>
#include <iola/model/iprogram.h>
#include <iola/model/isequence.h>
#include <iola/model/isource.h>

#include "About.h"
#include "MainWindow.h"
#include "ProgramMonitor.h"
#include "SequenceSettings.h"
#include "SourceMonitor.h"

#include "theme.h"

namespace iola
{
namespace gui
{

MainWindow::MainWindow() :
	Fl_Double_Window(Fl::w(), Fl::h(), "Iola NLE"),
	m_bRun(false)
{
	// Menu
	Fl_Menu_Bar* pkMenuBar = new Fl_Menu_Bar(0, 0, Fl::w(), 25);
	pkMenuBar->clear();
	pkMenuBar->box(FL_FLAT_BOX);
	pkMenuBar->textsize(11);
	pkMenuBar->add("&File/&New", FL_CTRL+'n', (Fl_Callback *)new_project, this, 0);
	pkMenuBar->add("&File/&Open...", FL_CTRL+'o', (Fl_Callback *)open_project, this, 0);
	pkMenuBar->add("&File/&Save", FL_CTRL+'s', (Fl_Callback *)save_project, this, 0);
	pkMenuBar->add("&File/_Save &As...", 0, (Fl_Callback *)save_as_project, this, 0);
	// pkMenuBar->add("&File/Export/Sequence as EDL...", 0, (Fl_Callback *)export_edl, this, 0);
	pkMenuBar->add("&File/_Sequence settings", 0, (Fl_Callback *)sequence_settings, this, 0);
	pkMenuBar->add("&File/&Quit", FL_CTRL+'q', (Fl_Callback *)quit_application, this, 0);
//	pkMenuBar->add("&Edit/&Undo", FL_CTRL+'z', 0, this, 0);
//	pkMenuBar->add("&Edit/&Redo", FL_CTRL+'y', 0, this, 0);
	pkMenuBar->add("&Help/&About Iola NLE", 0, (Fl_Callback *)about_iola, this, 0);

	// Layout
	Fl_Pack* pkMainGroup = new Fl_Pack(0, 25, w(), h()-45);
	Fl_Tile* pkDesktopGroup = new Fl_Tile(0, 0, w(), pkMainGroup->h());
	Fl_Group* pkMonitorGroup = new Fl_Group(0, 0, pkDesktopGroup->w(), pkDesktopGroup->h()/5*3);

	m_pkSourceMonitor = new SourceMonitor(0, 0, pkMonitorGroup->w()/2, pkMonitorGroup->h());
	m_pkProgramMonitor = new ProgramMonitor(pkMonitorGroup->w()/2, 0, pkMonitorGroup->w()/2, pkMonitorGroup->h());
	Fl_Box* timeline = new Fl_Box(FL_THIN_UP_BOX, 0, pkMonitorGroup->h(), w(), pkDesktopGroup->h()-pkMonitorGroup->h(), "");

	m_pkStatusbar = new Fl_Output(0, 0, w(), 20);
	m_pkStatusbar->color(FL_BACKGROUND_COLOR);
	m_pkStatusbar->textsize(10);
	m_pkStatusbar->textcolor(FL_LIGHT3);

	pkMonitorGroup->type(Fl_Pack::HORIZONTAL);
	pkMonitorGroup->box(FL_NO_BOX);
	pkMonitorGroup->add(m_pkSourceMonitor);
	pkMonitorGroup->add(m_pkProgramMonitor);
	pkMonitorGroup->end();

	pkDesktopGroup->box(FL_NO_BOX);
	pkDesktopGroup->add(pkMonitorGroup);
	pkDesktopGroup->add(timeline);
	pkDesktopGroup->end();

	pkMainGroup->add(pkDesktopGroup);
	pkMainGroup->add(m_pkStatusbar);
	pkMainGroup->resizable(pkDesktopGroup);
	pkMainGroup->end();

	// Dialogs
	m_pkSequenceSettings = new SequenceSettings();
	m_pkAbout = new About();

	// Window
	xclass("iola");
	callback((Fl_Callback *)quit_application, this);
	resizable(pkMainGroup);
	size_range(800, 750);
	end();
}

MainWindow::~MainWindow()
{
	on_source_alert_connection.disconnect();
	on_program_alert_connection.disconnect();
	rDebug("%s: Application window destructed", __PRETTY_FUNCTION__);
}

void MainWindow::on_alert(std::string strMessage)
{
	m_pkStatusbar->value(strMessage.c_str());
	fl_beep();
}

void MainWindow::open_project()
{
	char* filename = fl_file_chooser("Open", "Sequence (*.xml)", NULL);
	if (filename)
	{
		m_kProjectPath = boost::filesystem::path(filename);
		m_pkModel->program()->load_file(m_kProjectPath);
	}
}

void MainWindow::save_project()
{
	if (m_kProjectPath.empty())
		save_as_project();
	else
		m_pkModel->program()->save_file(m_kProjectPath);
}

void MainWindow::save_as_project()
{
	char* filename = fl_file_chooser("Save", "Sequence (*.xml)", NULL);
	if (filename)
	{
		m_kProjectPath = boost::filesystem::path(filename);
		m_pkModel->program()->save_file(m_kProjectPath);
	}
}

void MainWindow::clear_project()
{
	m_pkModel->source()->clear();
	m_pkModel->program()->clear();
}

void MainWindow::export_edl()
{
	char* filename = fl_file_chooser("Export", "EDL (*.edl)", NULL);
	if (filename)
	{
		boost::filesystem::path kEDLPath(filename);
		m_pkModel->program()->save_file(kEDLPath);
	}
}

void MainWindow::sequence_settings()
{
	m_pkSequenceSettings->connect_to(m_pkModel->program()->sequence()->video_settings());
	m_pkSequenceSettings->show();
}

void MainWindow::quit_application()
{
	if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape)
		return;

	iola::application::get_instance()->quit();
}

void MainWindow::about_iola()
{
	m_pkAbout->show();
}

void  MainWindow::connect_to(iola::model::imodel* model)
{
	if (!model)
	{
		rError("%s: Model passed as parameter is NULL", __PRETTY_FUNCTION__);
		return;
	}

	m_pkModel = model;
	m_pkSourceMonitor->connect_to(m_pkModel);
	m_pkProgramMonitor->connect_to(m_pkModel);

	iola::model::isource* pkSource = m_pkModel->source();
	iola::model::iprogram* pkProgram = m_pkModel->program();

	on_source_alert_connection = pkSource->on_alert_signal.connect(
		boost::bind(&MainWindow::on_alert, this, _1)
		);
	on_program_alert_connection = pkProgram->on_alert_signal.connect(
		boost::bind(&MainWindow::on_alert, this, _1)
		);
}

void  MainWindow::show()
{
	Fl::visible_focus(0);
	Fl::visual(FL_DOUBLE|FL_RGB);
	iola::gui::scheme::theme();

	Fl_Double_Window::show();

	m_bRun = true;
	while(m_bRun)
		Fl::wait(0); //NOTE The zero time makes the GUI really responsive
}

void  MainWindow::stop()
{
	m_bRun = false;
}

} // namespace gui
} // namespace iola
