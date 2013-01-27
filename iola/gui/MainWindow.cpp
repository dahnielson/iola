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

// QT
#include <QtGui>

// IOLA
#include <iola/model/imodel.h>
#include <iola/model/iprogram.h>
#include <iola/model/isource.h>

#include "MainWindow.h"

namespace iola
{
namespace gui
{

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
{
	// Create actions
	m_pkNewAction = new QAction(tr("&New"), this);
	m_pkNewAction->setShortcut(QKeySequence::New);
	connect(m_pkNewAction, SIGNAL(triggered()), this, SLOT(on_NewAction_triggered()));

	m_pkOpenAction = new QAction(tr("&Open..."), this);
	m_pkOpenAction->setShortcut(tr("Ctrl+O"));
	connect(m_pkOpenAction, SIGNAL(triggered()), this, SLOT(on_OpenAction_triggered()));

	m_pkSaveAction = new QAction(tr("&Save"), this);
	m_pkSaveAction->setShortcut(tr("Ctrl+S"));
	connect(m_pkSaveAction, SIGNAL(triggered()), this, SLOT(on_SaveAction_triggered()));

	m_pkSaveAsAction = new QAction(tr("Save &As..."), this);
	connect(m_pkSaveAsAction, SIGNAL(triggered()), this, SLOT(on_SaveAsAction_triggered()));

	m_pkSequenceSettingsAction = new QAction(tr("Sequence Settings..."), this);
	connect(m_pkSequenceSettingsAction, SIGNAL(triggered()), this, SLOT(on_SequenceSettingsAction_triggered()));

	m_pkQuitAction = new QAction(tr("&Quit"), this);
	m_pkQuitAction->setShortcut(tr("Ctrl+Q"));
	connect(m_pkQuitAction, SIGNAL(triggered()), this, SLOT(on_QuitAction_triggered()));

	// Create menus
	m_pkFileMenu = menuBar()->addMenu(tr("&File"));
	m_pkFileMenu->addAction(m_pkNewAction);
	m_pkFileMenu->addAction(m_pkOpenAction);
	m_pkFileMenu->addAction(m_pkSaveAction);
	m_pkFileMenu->addAction(m_pkSaveAsAction);
	m_pkFileMenu->addSeparator();
	m_pkFileMenu->addAction(m_pkSequenceSettingsAction);
	m_pkFileMenu->addSeparator();
	m_pkFileMenu->addAction(m_pkQuitAction);
}

MainWindow::~MainWindow()
{
	rDebug("%s: Application window destructed", __PRETTY_FUNCTION__);
}

void MainWindow::connect_to(iola::model::imodel* model)
{
	if (!model)
	{
		rError("%s: Model passed as parameter is NULL", __PRETTY_FUNCTION__);
		return;
	}

	m_pkModel = model;

	iola::model::isource* pkSource = m_pkModel->source();
	iola::model::iprogram* pkProgram = m_pkModel->program();

	on_source_alert_connection = pkSource->on_alert_signal.connect(
		boost::bind(&MainWindow::on_alert, this, _1)
		);
	on_program_alert_connection = pkProgram->on_alert_signal.connect(
		boost::bind(&MainWindow::on_alert, this, _1)
		);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (okToContinue())
		event->accept();
	else
		event->ignore();
}

bool MainWindow::okToContinue()
{
	if (isWindowModified())
	{
		int r = QMessageBox::warning(
			this,
			tr("Sequence"),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
			);

		if (r == QMessageBox::Yes)
			on_SaveAction_triggered();
		else if (r == QMessageBox::Cancel)
			return false;
	}

	return true;
}

void MainWindow::on_alert(std::string strMessage)
{
	statusBar()->showMessage(QString(strMessage.c_str()));
}

void MainWindow::on_NewAction_triggered()
{
	if (okToContinue())
	{
		m_pkModel->source()->clear();
		m_pkModel->program()->clear();
	}
}

void MainWindow::on_OpenAction_triggered()
{
	if (okToContinue())
	{
		QString filename = QFileDialog::getOpenFileName(
			this,
			tr("Open Sequence"), ".",
			tr("Sequence (*.xml)"), 0, 
			QFileDialog::DontUseNativeDialog
			);

		if (!filename.isEmpty())
		{
			m_kProjectPath = boost::filesystem::path(filename.toStdString());
			m_pkModel->program()->load_file(m_kProjectPath);
			setWindowModified(false);
		}
	}
}

void MainWindow::on_SaveAction_triggered()
{
	if (m_kProjectPath.empty())
	{
		on_SaveAsAction_triggered();
	}
	else
	{
		m_pkModel->program()->save_file(m_kProjectPath);
		setWindowModified(false);
	}
}

void MainWindow::on_SaveAsAction_triggered()
{
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Sequence"), ".",
		tr("Sequence (*.xml)"), 0, 
		QFileDialog::DontUseNativeDialog
		);

	if (!filename.isEmpty())
	{
		m_kProjectPath = boost::filesystem::path(filename.toStdString());
		m_pkModel->program()->save_file(m_kProjectPath);
		setWindowModified(false);
	}
}

void MainWindow::on_SequenceSettingsAction_triggered()
{
}

void MainWindow::on_QuitAction_triggered()
{
	close();
}

} // namespace gui
} // namespace iola
