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

// QT
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

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

//protected:
//	void closeEvent(QCloseEvent* event);

private:
	Ui::MainWindow* m_pkUI;

private slots:
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSaveAs_triggered();
	void on_actionSequenceSettings_triggered();
	void on_actionQuit_triggered();
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_MAINWINDOW_H
