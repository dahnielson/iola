/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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

// RLOG
#include <rlog/rlog.h>

// IOLA
#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace iola
{
namespace gui
{

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_pkUI(new Ui::MainWindow)
{
	m_pkUI->setupUi(this);
}

MainWindow::~MainWindow()
{
	rDebug("%s: Application window destructed", __PRETTY_FUNCTION__);
	delete m_pkUI;
}

void MainWindow::connect_to(iola::model::imodel* model)
{
	if (!model)
	{
		rError("%s: Model passed as parameter is NULL", __PRETTY_FUNCTION__);
		return;
	}

	m_pkModel = model;
}

//void closeEvent(QCloseEvent* event)
//{
//}

void MainWindow::on_actionNew_triggered()
{
}

void MainWindow::on_actionOpen_triggered()
{
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::on_actionSaveAs_triggered()
{
}

void MainWindow::on_actionSequenceSettings_triggered()
{
}

void MainWindow::on_actionQuit_triggered()
{
}

} // namespace gui
} // namespace iola
