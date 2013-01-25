/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_MAINWINDOW_H
#define IOLA_GUI_MAINWINDOW_H

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

// STD
#include <string>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

// FLTK
#include "fltk.h"

// IOLA
#include <iola/gui/igui.h>
#include <iola/model/imodel.h>

namespace iola
{
namespace gui
{

class About;
class SequenceSettings;
class ProgramMonitor;
class SourceMonitor;

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::MainWindow

/// Application main window
class MainWindow :
	public Fl_Double_Window,
	public iola::gui::igui
{
public:
	MainWindow();
	~MainWindow();

	/// Open (show file dialog)
	void open_project();
	/// Save
	void save_project();
	/// Save as (show file dialog)
	void save_as_project();
	/// New
	void clear_project();
	/// Export EDL (show file dialog)
	void export_edl();
	/// Open sequence settings dialog
	void sequence_settings();
	/// Quit
	void quit_application();
	/// Show about dialog
	void about_iola();

	void connect_to(iola::model::imodel* model);
	void show();
	void stop();

private:
	void on_alert(std::string strMessage);

	bool m_bRun;

	iola::model::imodel* m_pkModel;

	SourceMonitor* m_pkSourceMonitor;
	ProgramMonitor* m_pkProgramMonitor;
	Fl_Output* m_pkStatusbar;

	SequenceSettings* m_pkSequenceSettings;
	About* m_pkAbout;

	boost::filesystem::path m_kProjectPath;

	boost::signals2::connection on_source_alert_connection;
	boost::signals2::connection on_program_alert_connection;

	static void new_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->clear_project(); }
	static void open_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->open_project(); }
	static void save_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->save_project(); }
	static void save_as_project(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->save_as_project(); }
	static void export_edl(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->export_edl(); }
	static void sequence_settings(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->sequence_settings(); }
	static void quit_application(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->quit_application(); }
	static void about_iola(Fl_Widget*, void* v) { reinterpret_cast<MainWindow*>(v)->about_iola(); }
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_MAINWINDOW_H
