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
#include <iola/model/iaudio_settings.h>
#include <iola/model/iclip.h>
#include <iola/model/imarkers.h>
#include <iola/model/imarker_characteristics.h>
#include <iola/model/iplayhead.h>
#include <iola/model/isource.h>
#include <iola/model/ivideo_settings.h>
#include <iola/model/ivideo_characteristics.h>

#include <iola/model/clip.h>
#include <iola/model/iola_clip_reader.h>

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
	broadcast_command(iola::model::isource* source) :
		m_pkSource(source)
	{
		assert(m_pkSource);
	}

	~broadcast_command()
	{
	}

	void execute()
	{
		// Update video settings
		m_pkSource->on_width_change_signal(m_iWidth);
		m_pkSource->on_height_change_signal(m_iHeight);
		m_pkSource->on_dar_change_signal(m_kDAR);
		m_pkSource->on_par_change_signal(m_kPAR);
		m_pkSource->on_progressive_change_signal(m_bProgressive);
		m_pkSource->on_fps_change_signal(m_kFPS);
		m_pkSource->on_timebase_change_signal(m_iTimebase);
		m_pkSource->on_ntsc_change_signal(m_bNTSC);

		// Update audio settings
		m_pkSource->on_sample_depth_change_signal(m_iDepth);
		m_pkSource->on_sample_rate_change_signal(m_iRate);

		// Update markers
		m_pkSource->on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
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
	iola::model::isource* m_pkSource;

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
// class <unnamed>::source_implementation

class source_implementation :
		public iola::model::isource
{
public:
	source_implementation() :
		m_pkClip(0),
		m_pkConsumer(0),
		m_dSpeed(0)
	{
	}

	~source_implementation()
	{
		if (m_pkClip)
			delete m_pkClip;
	}

	///////////////////////////
	// isource

	//NOTE Silently fail if there's no clip,
	//     do not throw an exception into the GUI!

	void clear()
	{
		delete m_pkClip;
	}

	void load_clip(iola::model::iclip* clip)
	{
		if (!clip)
			throw std::invalid_argument("Clip passed as parameter is NULL");

		if (m_pkClip)
			delete m_pkClip;

		m_pkClip = clip;

		// Connect the clip signals to fire our signals
		m_pkClip->video_settings()->on_width_change_signal.connect(on_width_change_signal);
		m_pkClip->video_settings()->on_height_change_signal.connect(on_height_change_signal);
		m_pkClip->video_settings()->on_dar_change_signal.connect(on_dar_change_signal);
		m_pkClip->video_settings()->on_par_change_signal.connect(on_par_change_signal);
		m_pkClip->video_settings()->on_progressive_change_signal.connect(on_progressive_change_signal);
		m_pkClip->video_settings()->on_fps_change_signal.connect(on_fps_change_signal);
		m_pkClip->video_settings()->on_timebase_change_signal.connect(on_timebase_change_signal);
		m_pkClip->video_settings()->on_ntsc_change_signal.connect(on_ntsc_change_signal);
		m_pkClip->audio_settings()->on_sample_depth_change_signal.connect(on_sample_depth_change_signal);
		m_pkClip->audio_settings()->on_sample_rate_change_signal.connect(on_sample_rate_change_signal);
		m_pkClip->markers()->on_mark_change_signal.connect(on_mark_change_signal);
		m_pkClip->playhead()->on_playback_change_signal.connect(on_playback_change_signal);
		m_pkClip->on_producer_change_signal.connect(on_producer_change_signal);

		// Broadcast the clip settings
		broadcast_command* pkCommand = new broadcast_command(this);
		m_pkClip->audio_settings()->update_settings_on(pkCommand);
		m_pkClip->video_settings()->update_settings_on(pkCommand);
		m_pkClip->markers()->update_markers_on(pkCommand);
		pkCommand->execute();
		delete pkCommand;

		if (m_pkConsumer)
		{
			try
			{
				m_pkClip->connect_to(m_pkConsumer);
			}
			catch (std::exception& e)
			{
				rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			}
		}
	}

	iola::model::iclip* clip()
	{
		assert(m_pkClip);
		return m_pkClip;
	}

	///////////////////////////
	// ilodable

	//NOTE Silently fail if there's no clip,
	//     do not throw an exception into the GUI!

	void load_file(const boost::filesystem::path file)
	{
		iola::model::iclip* pkClip = iola::model::create_clip();
		iola::model::iclip_reader* pkReader = iola::model::create_iola_clip_reader(file);
		assert(pkClip);
		assert(pkReader);
		try
		{
			pkReader->read_file_into(pkClip);
			load_clip(pkClip);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
		delete pkReader;
	}

	///////////////////////////
	// iconsumable

	//NOTE Silently fail if there's no clip,
	//     do not throw an exception into the GUI!

	void connect_to(Mlt::Consumer* consumer)
	{
		if (!consumer)
			throw std::invalid_argument("Consumer passed as parameter is NULL");

		m_pkConsumer = consumer;

		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->connect_to(m_pkConsumer);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
		}
	}

	///////////////////////////
	// iplayable

	//NOTE Silently fail if there's no clip,
	//     do not throw an exception into the GUI!

	void goto_frame(const int frame)
	{
		if (!m_pkClip)
			return;

		m_pkClip->playhead()->set_frame(frame);
	}

	void goto_start()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->goto_start_on(m_pkClip->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_end()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->goto_end_on(m_pkClip->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void play_forward()
	{
		if (!m_pkClip)
			return;

		if (m_dSpeed == 0 || m_dSpeed + 1 == 0)
			m_dSpeed = 1;
		else if (m_dSpeed < 0)
			m_dSpeed /= 2;
		else if (m_dSpeed < 8)
			m_dSpeed *= 2;

		try
		{
			m_pkClip->playhead()->set_speed(m_dSpeed);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void play_backward()
	{
		if (!m_pkClip)
			return;

		if (m_dSpeed == 0 || m_dSpeed - 1 == 0)
			m_dSpeed = -1;
		else if (m_dSpeed > 0)
			m_dSpeed /= 2;
		else if (m_dSpeed > -8)
			m_dSpeed *= 2;

		try
		{
			m_pkClip->playhead()->set_speed(m_dSpeed);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void step_forward()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->playhead()->set_speed(0);
			m_pkClip->playhead()->set_relative_frame(+1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void step_backward()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->playhead()->set_speed(0);
			m_pkClip->playhead()->set_relative_frame(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void stop()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->playhead()->set_speed(0);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	///////////////////////////
	// imarkable 

	//NOTE Silently fail if there's no clip,
	//     do not throw an exception into the GUI!

	void mark_in()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->playhead()->set_in_point_on(m_pkClip->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void mark_out()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->playhead()->set_out_point_on(m_pkClip->markers());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void mark_clip()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->set_start_on(m_pkClip->markers()); //NOTE clip sets the in/out
			m_pkClip->set_end_on(m_pkClip->markers());   //     points here (cf. clip)
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void clear_mark_in()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->markers()->set_in_point(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void clear_mark_out()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->markers()->set_out_point(-1);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_mark_in()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->markers()->goto_in_point_on(m_pkClip->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

	void goto_mark_out()
	{
		if (!m_pkClip)
			return;

		try
		{
			m_pkClip->markers()->goto_out_point_on(m_pkClip->playhead());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			on_alert_signal(e.what());
		}
	}

private:
	iola::model::iclip* m_pkClip;
	Mlt::Consumer* m_pkConsumer;
	double m_dSpeed;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_source

isource* create_source()
{
	return new source_implementation();
}

} // namespace model
} // namespace iola
