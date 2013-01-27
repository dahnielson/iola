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

// QT
#include <QMainWindow>
#include <QCloseEvent>

// IOLA
#include <iola/model/imodel.h>

namespace iola
{
namespace gui
{

////////////////////////////////////////////////////////////////////////////
// class iola::gui::MainWindow

/// Application main window
class MainWindow :
	public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

	void connect_to(iola::model::imodel* model);

protected:
	void closeEvent(QCloseEvent* event);

private:
	bool okToContinue();
	void on_alert(std::string strMessage);

	QAction* m_pkNewAction;
	QAction* m_pkOpenAction;
	QAction* m_pkSaveAction;
	QAction* m_pkSaveAsAction;
	QAction* m_pkSequenceSettingsAction;
	QAction* m_pkQuitAction;

	QMenu* m_pkFileMenu;

	iola::model::imodel* m_pkModel;
	boost::filesystem::path m_kProjectPath;

	boost::signals2::connection on_source_alert_connection;
	boost::signals2::connection on_program_alert_connection;

private slots:
	void on_NewAction_triggered();
	void on_OpenAction_triggered();
	void on_SaveAction_triggered();
	void on_SaveAsAction_triggered();
	void on_SequenceSettingsAction_triggered();
	void on_QuitAction_triggered();
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_MAINWINDOW_H
