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

// STD
#include <cassert>
#include <pthread.h>

// LIBAO
#include <ao/ao.h>

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include <iola/application/iapplication.h>
#include <iola/gui/igui.h>
#include <iola/model/imodel.h>
#include <iola/model/iprogram.h>
#include <iola/model/isequence.h>

#include <iola/consumer/consumer_iola.h>
#include <iola/gui/gui.h>
#include <iola/model/model.h>
#include <iola/model/sequence.h>

namespace
{

/////////////////////////////////////////////////////////////////////////////
// global instances

pthread_mutex_t g_instance_mutex = PTHREAD_MUTEX_INITIALIZER;
static iola::application::iapplication* g_pkApplicationInstance;


/////////////////////////////////////////////////////////////////////////////
// application_implementation

class application_implementation :
	public iola::application::iapplication
{
public:
	application_implementation() :
		m_pkGUI(0),
		m_pkModel(0)
	{
	}

	~application_implementation()
	{}

	void run()
	{
		// Initialize audio
		ao_initialize();

		// Initialize MLT
		Mlt::Repository* pkRepository = Mlt::Factory::init(NULL);
		pkRepository->register_service(consumer_type, "iola", iola::consumer::consumer_iola_init);

		// Instance model
		m_pkModel = iola::model::create_model();
		assert(m_pkModel);

		// New sequence
		iola::model::isequence* pkSequence = iola::model::create_sequence();
		assert(pkSequence);
		m_pkModel->program()->load_sequence(pkSequence);

		// Instance GUI
		m_pkGUI = iola::gui::create_gui();
		assert(m_pkGUI);
		m_pkGUI->connect_to(m_pkModel);

		// Enter main loop
		m_pkGUI->show();

		// Clean up
		delete m_pkGUI;
		delete m_pkModel;

		// Shutdown audio
		ao_shutdown();
	}

	// Actions
	void quit()
	{
		if (m_pkGUI)
			m_pkGUI->stop();
	}

private:
	iola::gui::igui* m_pkGUI;
	iola::model::imodel* m_pkModel;
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
	pthread_mutex_lock(&g_instance_mutex);
	if (!g_pkApplicationInstance)
		g_pkApplicationInstance = new application_implementation();
	pthread_mutex_unlock(&g_instance_mutex);

	return g_pkApplicationInstance;
}

} // namespace application
} // namespace iola
