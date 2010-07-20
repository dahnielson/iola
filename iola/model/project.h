/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_PROJECT_H
#define IOLA_MODEL_PROJECT_H

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
#include "timecode.h"

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// class iola::model::project

class project :
	public iola::model::iproject
{
public:
	project();
	~project();

	// Profile
	Mlt::Profile& get_profile();

	// Storage Aspect Ratio
	void set_width(int width);
	int get_width();
	void set_height(int height);
	int get_height();

	// Display Aspect Ratio
	int get_dar_num();
	int get_dar_den();

	// Pixel Aspect Ratio
	void set_par(iola::model::iproject::par_t par);
	iola::model::iproject::par_t get_par();
	int get_par_num();
	int get_par_den();

	// Anamorphic
	void set_anamorphic(bool anamorphic);
	bool get_anamorphic();

	// Field Dominance
	void set_field_dominance(iola::model::iproject::field_t dominance);
	iola::model::iproject::field_t get_field_dominance();
	bool get_progressive();

	// Frame Rate
	void set_fps_timebase(int timebase);
	int get_fps_timebase();
	void set_fps_ntsc(bool ntsc);
	bool get_fps_ntsc();
	int get_fps_num();
	int get_fps_den();

	// Audio
	void set_sample_depth(int depth);
	int get_sample_depth();
	void set_sample_rate(int rate);
	int get_sample_rate();

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
	void source_set_mark_in(int frame);
	void source_set_mark_out();
	void source_set_mark_out(int frame);
	void source_set_mark_clip();
	void source_clear_mark_in();
	void source_clear_mark_out();
	void source_goto_mark_in();
	void source_goto_mark_out();
	void source_goto_start();
	void source_goto_end();

	// Program
	void program_connect_consumer(Mlt::Consumer* consumer);

	void program_set_name(std::string name);
	std::string program_get_name();

	void program_set_start_timecode(std::string timecode);
	std::string program_get_start_timecode();

	void program_new();
	void program_load(boost::filesystem::path sequence);
	void program_save(boost::filesystem::path sequence);
	void program_export_edl(boost::filesystem::path edl);
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
	void program_set_mark_in(int frame);
	void program_set_mark_out();
	void program_set_mark_out(int frame);
	void program_set_mark_cut();
	void program_clear_mark_in();
	void program_clear_mark_out();
	void program_goto_mark_in();
	void program_goto_mark_out();
	void program_goto_previous_edit();
	void program_goto_next_edit();
	void program_goto_start();
	void program_goto_end();
	void program_lift();
	void program_lift(const int program_in, const int program_out);
	void program_extract();
	void program_extract(const int program_in, const int program_out);
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

	// Sequence settings
	int m_iWidth;
	int m_iHeight;
	iola::model::iproject::par_t m_iPAR;
	bool m_bAnamorphic;
	iola::model::iproject::field_t m_iFieldDominance;
	int m_iTimebase;
	bool m_bNTSC;
	int m_iSampleDepth;
	int m_iSampleRate;
	std::string m_strSequenceName;
	timecode m_pkSequenceStart;

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

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_PROJECT_H
