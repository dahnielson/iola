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

// FLTK
#include <FL/Fl.H>

// LIBAO
#include <ao/ao.h>

// IOLA
#include <iola/gui/MainWindow.h>
#include <iola/domain/iproject.h>
#include <iola/domain/project.h>
#include "get_instance.h"

namespace
{

/////////////////////////////////////////////////////////////////////////////
// global instance

static iola::application::iapplication* g_pkApplicationInstance;

/////////////////////////////////////////////////////////////////////////////
// application_implementation

class application_implementation :
	public iola::application::iapplication
{
public:
	application_implementation() :
		m_bRun(false),
		m_pkGUI(0),
		m_pkProject(0)
	{
	}

	~application_implementation()
	{}

	iola::domain::iproject* get_project()
	{
		return m_pkProject;
	}

	void run()
	{
		ao_initialize();

		// Instance project
		m_pkProject = new iola::domain::project();

		// Instance GUI
		m_pkGUI = new iola::gui::MainWindow();

		m_bRun = true;
		m_pkGUI->show();

		while(m_bRun)
			execute();

		ao_shutdown();
	}

	// Actions
	void quit()
	{
		m_bRun = false;
	}

private:
	void execute()
	{
		Fl::wait();
	}

	bool m_bRun;
	iola::gui::MainWindow* m_pkGUI;
	iola::domain::iproject* m_pkProject;
};


} // namespace

namespace iola
{
namespace application
{

/////////////////////////////////////////////////////////////////////////////
// function iola::application::get_instance

iapplication* get_instance()
{
	if (!g_pkApplicationInstance)
		g_pkApplicationInstance = new application_implementation();

	return g_pkApplicationInstance;
}

} // namespace application
} // namespace iola
