/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

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
#include <exception>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/rational.hpp>

// RLOG
#include <rlog/rlog.h>

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include <iola/icommand.h>
#include <iola/model/iaudio_characteristics.h>
#include <iola/model/iclip.h>
#include <iola/model/imarkers.h>
#include <iola/model/imarker_characteristics.h>
#include <iola/model/iplayhead.h>
#include <iola/model/iprogram.h>
#include <iola/model/isequence.h>
#include <iola/model/isequence_reader.h>
#include <iola/model/isequence_writer.h>
#include <iola/model/ivideo_characteristics.h>

#include <iola/model/sequence.h>
#include <iola/model/iola_sequence_reader.h>
#include <iola/model/iola_sequence_writer.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::broadcast_command

class broadcast_command :
	public iola::icommand,
	public iola::model::iaudio_characteristics,
	public iola::model::ivideo_characteristics,
	public iola::model::imarker_characteristics
{
public:
	broadcast_command(iola::model::iprogram* program) :
		m_pkProgram(program)
	{
		assert(m_pkProgram);
	}

	~broadcast_command()
	{
	}

	void execute()
	{
		// Update video settings
		m_pkProgram->on_width_change_signal(m_iWidth);
		m_pkProgram->on_height_change_signal(m_iHeight);
		m_pkProgram->on_dar_change_signal(m_kDAR);
		m_pkProgram->on_par_change_signal(m_kPAR);
		m_pkProgram->on_progressive_change_signal(m_bProgressive);
		m_pkProgram->on_fps_change_signal(m_kFPS);
		m_pkProgram->on_timebase_change_signal(m_iTimebase);
		m_pkProgram->on_ntsc_change_signal(m_bNTSC);

		// Update audio settings
		m_pkProgram->on_sample_depth_change_signal(m_iDepth);
		m_pkProgram->on_sample_rate_change_signal(m_iRate);

		// Update markers
		m_pkProgram->on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	///////////////////////////
	// iaudio_characteristics

	void set_sample_depth(const int depth)
	{
		m_iDepth = depth;
	}

	void set_sample_rate(const int rate)
	{
		m_iRate = rate;
	}

	///////////////////////////
	// ivideo_characteristics

	void set_width(const int width)
	{
		m_iWidth = width;
	}

	void set_height(const int height)
	{
		m_iHeight = height;
	}

	void set_dar(const boost::rational<int> dar)
	{
		m_kDAR = dar;
	}

	void set_par(const boost::rational<int> par)
	{
		m_kPAR = par;
	}

	void set_fps(const boost::rational<int> fps)
	{
		m_kFPS = fps;
	}

	void set_progressive(const bool progressive)
	{
		m_bProgressive = progressive;
	}

	///////////////////////////
	// imarker_characteristics

	void set_markers(const int in, const int out, const int start, const int end)
	{
		m_iIn = in;
		m_iOut = out;
		m_iStart = start;
		m_iEnd = end;
	}

private:
	iola::model::iprogram* m_pkProgram;

	int m_iDepth;
	int m_iRate;

	int m_iWidth;
	int m_iHeight;
	boost::rational<int> m_kDAR;
	boost::rational<int> m_kPAR;
	bool m_bProgressive;
	boost::rational<int> m_kFPS;
	int m_iTimebase;
	bool m_bNTSC;

	int m_iIn;
	int m_iOut;
	int m_iStart;
	int m_iEnd;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::three_point_edit_command

/// The three-point edit command will sanity-check and correct the in/out/start/end points.
class threepoint_edit_command :
	public iola::icommand,
	public iola::model::imarker_characteristics
{
public:
	threepoint_edit_command(iola::model::imarkers* markers) :
		m_pkMarkers(markers),
		m_iIn(-1),
		m_iOut(-1),
		m_iStart(-1),
		m_iEnd(-1)
	{
		assert(m_pkMarkers);
	}

	~threepoint_edit_command()
	{
	}

	void execute()
	{
		int program_in, program_out, source_in, source_out;
		
		if (m_iStart != -1 && m_iEnd != -1 &&
		    m_iIn != -1 && m_iOut != -1 &&
		    m_iStart < m_iEnd)
		{
			rDebug("%s: Four-point editing: start/end takes precedence", __PRETTY_FUNCTION__);
			const int duration = m_iEnd - m_iStart;
			program_in = m_iStart;
			program_out = m_iEnd;
			source_in = m_iIn;
			source_out = m_iIn + duration;
		}
		else if (m_iStart != -1 && m_iEnd != -1 &&
			 m_iIn != -1 && m_iOut == -1 &&
			 m_iStart < m_iEnd)
		{
			rDebug("%s: Three-point editing: in/start/end used", __PRETTY_FUNCTION__);
			const int duration = m_iEnd - m_iStart;
			program_in = m_iStart;
			program_out = m_iEnd;
			source_in = m_iIn;
			source_out = m_iIn + duration;
		}
		else if (m_iStart != -1 && m_iEnd != -1 &&
			 m_iIn == -1 && m_iOut != -1 &&
			 m_iStart < m_iEnd)
		{
			rDebug("%s: Three-point editing: out/start/end used", __PRETTY_FUNCTION__);
			const int duration = m_iEnd - m_iStart;
			program_in = m_iStart;
			program_out = m_iEnd;
			source_in = m_iOut - duration;
			source_out = m_iOut;
		}
		else if (m_iStart != -1 && m_iEnd == -1 &&
			 m_iIn != -1 && m_iOut != -1 &&
			 m_iIn < m_iOut)
		{
			rDebug("%s: Three-point editing: in/out/start used", __PRETTY_FUNCTION__);
			const int duration = m_iOut - m_iIn;
			program_in = m_iStart;
			program_out = m_iStart + duration;
			source_in = m_iIn;
			source_out = m_iOut;
		}
		else if (m_iStart == -1 && m_iEnd != -1 &&
			 m_iIn != -1 && m_iOut != -1 &&
			 m_iIn < m_iOut)
		{
			rDebug("%s: Three-point editing: in/out/end used", __PRETTY_FUNCTION__);
			const int duration = m_iOut - m_iIn;
			program_in = m_iEnd - duration;
			program_out = m_iEnd;
			source_in = m_iIn;
			source_out = m_iOut;
		}
		else
		{
			throw std::range_error("No good in/out/start/end points found");
		}

		assert(m_pkMarkers);
		m_pkMarkers->set_in_point(source_in);
		m_pkMarkers->set_out_point(source_out);
		m_pkMarkers->set_start_point(program_in);
		m_pkMarkers->set_end_point(program_out);

	}

	///////////////////////////
	// imarker_characteristics

	void set_markers(const int in, const int out, const int start, const int end)
	{
		m_iIn = in;
		m_iOut = out;
		m_iStart = start;
		m_iEnd = end;
	}

private:
	iola::model::imarkers* m_pkMarkers;
	int m_iIn;
	int m_iOut;
	int m_iStart;
	int m_iEnd;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::program_implementation

class program_implementation :
	public iola::model::iprogram
{
public:
	program_implementation() :
		m_pkSequence(0),
		m_pkConsumer(0),
		m_dSpeed(0)
	{
	}

	~program_implementation()
	{
		if (m_pkSequence)
			delete m_pkSequence;
	}

	///////////////////////////
	// iprogram

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void clear()
	{
		if (m_pkSequence)
			delete m_pkSequence;
	}

	void load_sequence(iola::model::isequence* sequence)
	{
		if (!sequence)
			throw std::invalid_argument("Sequence passed as parameter is NULL");

		if (m_pkSequence)
			delete m_pkSequence;

		m_pkSequence = sequence;

		// Connect the sequence signals to fire our signals
		m_pkSequence->video_settings()->on_width_change_signal.connect(on_width_change_signal);
		m_pkSequence->video_settings()->on_height_change_signal.connect(on_height_change_signal);
		m_pkSequence->video_settings()->on_dar_change_signal.connect(on_dar_change_signal);
		m_pkSequence->video_settings()->on_par_change_signal.connect(on_par_change_signal);
		m_pkSequence->video_settings()->on_progressive_change_signal.connect(on_progressive_change_signal);
		m_pkSequence->video_settings()->on_fps_change_signal.connect(on_fps_change_signal);
		m_pkSequence->video_settings()->on_timebase_change_signal.connect(on_timebase_change_signal);
		m_pkSequence->video_settings()->on_ntsc_change_signal.connect(on_ntsc_change_signal);
		m_pkSequence->audio_settings()->on_sample_depth_change_signal.connect(on_sample_depth_change_signal);
		m_pkSequence->audio_settings()->on_sample_rate_change_signal.connect(on_sample_rate_change_signal);
		m_pkSequence->markers()->on_mark_change_signal.connect(on_mark_change_signal);
		m_pkSequence->playhead()->on_playback_change_signal.connect(on_playback_change_signal);
		m_pkSequence->on_producer_change_signal.connect(on_producer_change_signal);

		// Broadcast the clip settings
		broadcast_command* pkCommand = new broadcast_command(this);
		m_pkSequence->audio_settings()->update_settings_on(pkCommand);
		m_pkSequence->video_settings()->update_settings_on(pkCommand);
		m_pkSequence->markers()->update_markers_on(pkCommand);
		pkCommand->execute();
		delete pkCommand;

		if (m_pkConsumer)
		{
			try
			{
				m_pkSequence->connect_to(m_pkConsumer);
			}
			catch (std::exception& e)
			{
				rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			}
		}
	}

	iola::model::isequence* sequence()
	{
		assert(m_pkSequence);
		return m_pkSequence;
	}

	void goto_previous_edit()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->goto_previous_edit_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_next_edit()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->goto_next_edit_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void make_lift_edit()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->cut_region(m_pkSequence->markers());
			m_pkSequence->insert_blank(m_pkSequence->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void make_extract_edit()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->cut_region(m_pkSequence->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void make_overwrite_edit(iola::model::iclip* clipitem)
	{
		if (!clipitem)
			throw std::invalid_argument("Clip passed as parameter is NULL");

		if (!m_pkSequence)
			return;

		threepoint_edit_command* pkCommand = new threepoint_edit_command(clipitem->markers());
		try
		{
			clipitem->markers()->set_start_point(-1);
			clipitem->markers()->set_end_point(-1);
			m_pkSequence->markers()->set_start_point_on(clipitem->markers());
			m_pkSequence->markers()->set_end_point_on(clipitem->markers());

			clipitem->markers()->update_markers_on(pkCommand);
			pkCommand->execute();

			m_pkSequence->cut_region(clipitem->markers());
			m_pkSequence->insert_clip(clipitem);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
		delete pkCommand;
	}

	void make_insert_edit(iola::model::iclip* clipitem)
	{
		if (!clipitem)
			throw std::invalid_argument("Clip passed as parameter is NULL");

		if (!m_pkSequence)
			return;

		threepoint_edit_command* pkCommand = new threepoint_edit_command(clipitem->markers());
		try
		{
			clipitem->markers()->set_start_point(-1);
			clipitem->markers()->set_end_point(-1);
			m_pkSequence->markers()->set_start_point_on(clipitem->markers());
			m_pkSequence->markers()->set_end_point_on(clipitem->markers());

			clipitem->markers()->update_markers_on(pkCommand);
			pkCommand->execute();

			m_pkSequence->insert_clip(clipitem);	
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
		delete pkCommand;
	}

	///////////////////////////
	// ilodable

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void load_file(const boost::filesystem::path file)
	{
		iola::model::isequence* pkSequence = iola::model::create_sequence();
		iola::model::isequence_reader* pkReader = iola::model::create_iola_sequence_reader(file);
		assert(pkSequence);
		assert(pkReader);
		try
		{
			pkReader->read_file_into(pkSequence);
			load_sequence(pkSequence);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
		delete pkReader;
	}

	///////////////////////////
	// isaveable

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void save_file(const boost::filesystem::path file)
	{
		if (!m_pkSequence)
			return;

		iola::model::isequence_writer* pkWriter = iola::model::create_iola_sequence_writer(file);
		try
		{
			m_pkSequence->write_sequence_to(pkWriter);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
		delete pkWriter;
	}

	///////////////////////////
	// iconsumable

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void connect_to(Mlt::Consumer* consumer)
	{
		if (!consumer)
			throw std::invalid_argument("Consumer passed as parameter is NULL");

		m_pkConsumer = consumer;

		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->connect_to(consumer);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
		}
	}

	///////////////////////////
	// iplayable

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void goto_frame(const int frame)
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_frame(frame);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_start()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->goto_start_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_end()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->goto_end_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void play_forward()
	{
		if (!m_pkSequence)
			return;

		if (m_dSpeed == 0 || m_dSpeed + 1 == 0)
			m_dSpeed = 1;
		else if (m_dSpeed < 0)
			m_dSpeed /= 2;
		else if (m_dSpeed < 8)
			m_dSpeed *= 2;

		try
		{
			m_pkSequence->playhead()->set_speed(m_dSpeed);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void play_backward()
	{
		if (!m_pkSequence)
			return;

		if (m_dSpeed == 0 || m_dSpeed - 1 == 0)
			m_dSpeed = -1;
		else if (m_dSpeed > 0)
			m_dSpeed /= 2;
		else if (m_dSpeed > -8)
			m_dSpeed *= 2;

		try
		{
			m_pkSequence->playhead()->set_speed(m_dSpeed);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void step_forward()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_speed(0);
			m_pkSequence->playhead()->set_relative_frame(+1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void step_backward()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_speed(0);
			m_pkSequence->playhead()->set_relative_frame(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void stop()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_speed(0);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	///////////////////////////
	// imarkable

	//NOTE Silently fail if there's no sequence,
	//     do not throw an exception into the GUI!

	void mark_in()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_start_point_on(m_pkSequence->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void mark_out()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->playhead()->set_end_point_on(m_pkSequence->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void mark_clip()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->set_start_on(m_pkSequence->markers()); //NOTE sequence sets the start/end
			m_pkSequence->set_end_on(m_pkSequence->markers());   //     points here (cf. clip)
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void clear_mark_in()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->markers()->set_start_point(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void clear_mark_out()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->markers()->set_end_point(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_mark_in()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->markers()->goto_start_point_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_mark_out()
	{
		if (!m_pkSequence)
			return;

		try
		{
			m_pkSequence->markers()->goto_end_point_on(m_pkSequence->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

private:
	iola::model::isequence* m_pkSequence;
	Mlt::Consumer* m_pkConsumer;
	double m_dSpeed;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_program

iprogram* create_program()
{
	return new program_implementation();
}

} // namespace model
} // namespace iola
