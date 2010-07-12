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

// STD
#include <pthread.h>

// FLTK
#include <FL/Fl.H>

// LIBAO
#include <ao/ao.h>

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include <iola/consumer/consumer_iola.h>
#include <iola/domain/iproject.h>
#include <iola/domain/project.h>
#include <iola/gui/MainWindow.h>
#include "get_instance.h"

namespace
{

/////////////////////////////////////////////////////////////////////////////
// global instance

pthread_mutex_t instance_mutex = PTHREAD_MUTEX_INITIALIZER;
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
		// Initialize audio
		ao_initialize();

		// Initialize MLT
		Mlt::Repository* pkRepos = Mlt::Factory::init(NULL);
		pkRepos->register_service(consumer_type, "iola", iola::consumer::consumer_iola_init);

		// Instance project
		m_pkProject = new iola::domain::project();

		// Instance GUI
		m_pkGUI = new iola::gui::MainWindow();

		// Enter main loop
		m_bRun = true;
		m_pkGUI->show();

		while(m_bRun)
			execute();

		// Clean up
		delete m_pkGUI;
		delete m_pkProject;

		// Shutdown audio
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
		Fl::wait(0); //NOTE The zero time makes the GUI really responsive
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
	pthread_mutex_lock(&instance_mutex);
	if (!g_pkApplicationInstance)
		g_pkApplicationInstance = new application_implementation();
	pthread_mutex_unlock(&instance_mutex);

	return g_pkApplicationInstance;
}

} // namespace application
} // namespace iola
