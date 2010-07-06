/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOMAIN_PROJECT_H
#define IOLA_DOMAIN_PROJECT_H

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

// BOOST
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include "iproject.h"

namespace iola
{
namespace domain
{

////////////////////////////////////////////////////////////////////////////
// class iola::domain::project

class project :
	public iola::domain::iproject
{
public:
	project();
	~project();

	Mlt::Profile& get_profile();

	// Source
	void source_connect_consumer(Mlt::Consumer* consumer);

	void source_new();
	void source_load(boost::filesystem::path clip);
	int source_get_start();
	int source_get_end();
	double source_get_speed();
	void source_set_speed(double speed);
	void source_seek(int position);

	void source_step_forward();
	void source_step_backward();
	void source_play_forward();
	void source_play_reverse();
	void source_pause();
	int source_get_mark_in();
	int source_get_mark_out();
	void source_set_mark_in();
	void source_set_mark_out();
	void source_set_mark_clip();
	void source_clear_mark_in();
	void source_clear_mark_out();
	void source_goto_mark_in();
	void source_goto_mark_out();
	void source_goto_start();
	void source_goto_end();

	// Program
	void program_connect_consumer(Mlt::Consumer* consumer);

	void program_new();
	void program_load(boost::filesystem::path sequence);
	void program_save(boost::filesystem::path sequence);
	int program_get_start();
	int program_get_end();
	void program_set_duration(int duration);
	int program_get_duration();
	void program_set_speed(double speed);
	double program_get_speed();
	void program_seek(int position);

	int program_get_clip_count();
	Mlt::ClipInfo* program_get_clip_info(int index);

	void program_step_forward();
	void program_step_backward();
	void program_play_forward();
	void program_play_reverse();
	void program_pause();
	int program_get_mark_in();
	int program_get_mark_out();
	void program_set_mark_in();
	void program_set_mark_out();
	void program_set_mark_cut();
	void program_clear_mark_in();
	void program_clear_mark_out();
	void program_goto_mark_in();
	void program_goto_mark_out();
	void program_goto_previous_edit();
	void program_goto_next_edit();
	void program_goto_start();
	void program_goto_end();
	void program_insert();
	void program_insert(boost::filesystem::path resource, const int program_in,
			    const int source_in, const int source_out);
	void program_overwrite();
	void program_overwrite(boost::filesystem::path resource, const int program_in,
			       const int source_in, const int source_out);

private:
	// The "model"
	Mlt::Playlist* m_pkSource;
	Mlt::Playlist* m_pkProgram;
	Mlt::Profile m_kProfile;

	Mlt::Event* m_pkSourceProducerChangeEvent;
	Mlt::Event* m_pkProgramProducerChangeEvent;

	static void source_producer_change_callback(mlt_producer, project* self)
	{
		self->on_source_producer_change_signal();
	}

	static void program_producer_change_callback(mlt_producer, project* self)
	{
		self->on_program_producer_change_signal();
	}
};

} // namespace domain
} // namespace iola

#endif // IOLA_DOMAIN_PROJECT_H
