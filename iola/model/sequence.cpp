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
#include <cassert>
#include <string>

// BOOST
#include <boost/filesystem.hpp>

// RLOG
#include <rlog/rlog.h>

// MLT
#include <mlt++/Mlt.h>

// IOLA
#include <iola/icommand.h>
#include <iola/model/iaudio_settings.h>
#include <iola/model/iclip.h>
#include <iola/model/ifile.h>
#include <iola/model/ifile_characteristics.h>
#include <iola/model/imarkers.h>
#include <iola/model/imarker_characteristics.h>
#include <iola/model/iplayhead.h>
#include <iola/model/isequence.h>
#include <iola/model/isequence_writer.h>
#include <iola/model/ivideo_characteristics.h>
#include <iola/model/ivideo_settings.h>

#include <iola/model/audio_settings.h>
#include <iola/model/clip.h>
#include <iola/model/markers.h>
#include <iola/model/video_settings.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::cut_region_command

class cut_region_command :
	public iola::icommand,
	public iola::model::imarker_characteristics
{
public:
	cut_region_command(Mlt::Playlist* playlist) :
		m_pkPlaylist(playlist),
		m_iStart(-1),
		m_iEnd(-1)
	{
		assert(m_pkPlaylist);
	}

	~cut_region_command()
	{
	}

	void execute()
	{
		if (m_iStart == -1 || m_iEnd == -1)
			throw std::runtime_error("Can't execute command, both start/end points must be set");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int clip_index = m_pkPlaylist->remove_region(m_iStart, m_iEnd - m_iStart);
		m_pkPlaylist->unlock();
	}

	///////////////////////////
	// imarker_characteristics

	void set_markers(const int in, const int out, const int start, const int end)
	{
		if (start > end)
			throw std::invalid_argument("Missmatched order between start/end points");

		m_iStart = start;
		m_iEnd = end;
	}

private:
	Mlt::Playlist* m_pkPlaylist;
	int m_iStart;
	int m_iEnd;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::insert_blank_command

class insert_blank_command :
	public iola::icommand,
	public iola::model::imarker_characteristics
{
public:
	insert_blank_command(Mlt::Playlist* playlist) :
		m_pkPlaylist(playlist),
		m_iStart(-1),
		m_iEnd(-1)
	{
		assert(m_pkPlaylist);
	}

	~insert_blank_command()
	{
	}

	void execute()
	{
		if (m_iStart == -1 || m_iEnd == -1)
			throw std::runtime_error("Cant't execute command, both start/end points must be set");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int clip_index = m_pkPlaylist->get_clip_index_at(m_iStart);
		m_pkPlaylist->split(clip_index, m_iStart - m_pkPlaylist->clip_start(clip_index));
		m_pkPlaylist->insert_blank(clip_index+1, m_iEnd - m_iStart);
		m_pkPlaylist->unlock();
	}

	///////////////////////////
	// imarker_characteristics

	void set_markers(const int in, const int out, const int start, const int end)
	{
		if (start > end)
			throw std::invalid_argument("Missmatched order between start/end points");

		m_iStart = start;
		m_iEnd = end;
	}

private:
	Mlt::Playlist* m_pkPlaylist;
	int m_iStart;
	int m_iEnd;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::insert_clip_command

class insert_clip_command :
	public iola::icommand,
	public iola::model::ifile_characteristics,
	public iola::model::imarker_characteristics,
	public iola::model::ivideo_characteristics
{
public:
	insert_clip_command(Mlt::Playlist* playlist) :
		m_pkPlaylist(playlist),
		m_kProfile(Mlt::Profile()),
		m_kFile(),
		m_iIn(-1),
		m_iOut(-1),
		m_iStart(-1),
		m_iEnd(-1)
	{
		assert(m_pkPlaylist);
	}

	~insert_clip_command()
	{
	}

	void execute()
	{
		if (m_iIn == -1 || m_iOut == -1 || m_iStart == -1 || m_iEnd == -1)
			throw std::runtime_error("Can't execute command, both in/out/start/end points must be set");

		Mlt::Producer* pkProducer = new Mlt::Producer(m_kProfile, m_kFile.string().c_str());
		pkProducer->set("meta.iola.in", m_iIn);
		pkProducer->set("meta.iola.out", m_iOut);
		pkProducer->set("meta.iola.start", m_iStart);
		pkProducer->set("meta.iola.end", m_iEnd);

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		if (m_pkPlaylist->get_length() < m_iEnd)
			m_pkPlaylist->blank(m_iEnd);

		const int clip_index = m_pkPlaylist->get_clip_index_at(m_iStart);
		m_pkPlaylist->split(clip_index, m_iStart - m_pkPlaylist->clip_start(clip_index));
		m_pkPlaylist->insert(*pkProducer, clip_index+1, m_iIn, m_iOut);
		m_pkPlaylist->unlock();
	}

	///////////////////////////
	// ifile_characteristics

	void set_pathurl(boost::filesystem::path file) 
	{ 
		m_kFile = file; 
	}

	///////////////////////////
	// imarker_characteristics

	void set_markers(const int in, const int out, const int start, const int end)
	{
		if (in > out || start > end)
			throw std::invalid_argument("Missmatched order between in/out/start/end points");

		if (out - in == end - start)
			throw std::invalid_argument("Missmatched duration between in/out/start/end points");

		m_iIn = in;
		m_iOut = out;
		m_iStart = start;
		m_iEnd = end;
	}

	///////////////////////////
	// ivideo_characteristics

	void set_width(const int width)
	{
		m_kProfile.get_profile()->width = width;
	}

	void set_height(const int height)
	{
		m_kProfile.get_profile()->height = height;
	}

	void set_dar(const boost::rational<int> dar)
	{
		m_kProfile.get_profile()->display_aspect_num = dar.numerator();
		m_kProfile.get_profile()->display_aspect_den = dar.denominator();
	}

	void set_par(const boost::rational<int> par)
	{
		m_kProfile.get_profile()->sample_aspect_num = par.numerator();
		m_kProfile.get_profile()->sample_aspect_den = par.denominator();
	}

	void set_fps(const boost::rational<int> fps)
	{
		m_kProfile.get_profile()->frame_rate_num = fps.numerator();
		m_kProfile.get_profile()->frame_rate_den = fps.denominator();
	}

	void set_progressive(const bool progressive)
	{
		m_kProfile.get_profile()->progressive = progressive;
	}

private:
	Mlt::Playlist* m_pkPlaylist;
	Mlt::Profile m_kProfile;
	boost::filesystem::path m_kFile;
	int m_iIn;
	int m_iOut;
	int m_iStart;
	int m_iEnd;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::sequence_implementation

class sequence_implementation :
	public iola::model::isequence,
	public iola::model::iplayhead  //NOTE I pretend to be my own child
{
public:
	sequence_implementation() :
		m_strName(""),
		m_strUUID(""),
		m_iDuration(0),
		m_strTimecode("00:00:00:00"),
		m_pkMarkers(iola::model::create_markers()),
		m_pkVideoSettings(iola::model::create_video_settings()),
		m_pkAudioSettings(iola::model::create_audio_settings()),
		m_pkPlaylist(new Mlt::Playlist),
		m_kProfile(Mlt::Profile())
	{
		assert(m_pkMarkers);
		assert(m_pkVideoSettings);
		assert(m_pkAudioSettings);
		assert(m_pkPlaylist);
	}

	~sequence_implementation()
	{
		assert(m_pkMarkers);
		assert(m_pkVideoSettings);
		assert(m_pkAudioSettings);
		assert(m_pkPlaylist);
		delete m_pkMarkers;
		delete m_pkVideoSettings;
		delete m_pkAudioSettings;
		delete m_pkPlaylist;
	}

	void write_sequence_to(iola::model::isequence_writer* writer)
	{
		if (!writer)
			throw std::invalid_argument("Writer passed as parameter is NULL");

		assert(m_pkVideoSettings);
		assert(m_pkAudioSettings);
		assert(m_pkPlaylist);

		m_pkVideoSettings->update_settings_on(writer);
		m_pkAudioSettings->update_settings_on(writer);
		writer->set_name(m_strName);
		writer->set_duration(m_iDuration);
		writer->set_timecode(m_strTimecode);

		const int number_of_clips = m_pkPlaylist->count();
		for (int i = 0; i < number_of_clips; ++i)
		{
			Mlt::ClipInfo* pkInfo =  m_pkPlaylist->clip_info(i);
			iola::model::iclip* pkClipItem = iola::model::create_clip();
			pkClipItem->file()->set_pathurl(pkInfo->resource);
			pkClipItem->markers()->set_in_point(pkInfo->frame_in);
			pkClipItem->markers()->set_out_point(pkInfo->frame_out);
			pkClipItem->markers()->set_start_point(pkInfo->start);
			pkClipItem->markers()->set_out_point(pkInfo->start + pkInfo->frame_out - pkInfo->frame_in);
			writer->append_clipitem(pkClipItem);
			Mlt::Playlist::delete_clip_info(pkInfo);
		}

		writer->execute();
	}

	void cut_region(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		cut_region_command* pkCommand = new cut_region_command(m_pkPlaylist);
		try
		{
			markers->update_markers_on(pkCommand);
			pkCommand->execute();
			on_producer_change_signal(m_pkPlaylist->get_in(), m_pkPlaylist->get_out());
		}
		catch (std::exception& e)
		{
			delete pkCommand;
			throw;
		}
		delete pkCommand;
	}

	void insert_blank(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		insert_blank_command* pkCommand = new insert_blank_command(m_pkPlaylist);
		try
		{
			markers->update_markers_on(pkCommand);
			pkCommand->execute();
			on_producer_change_signal(m_pkPlaylist->get_in(), m_pkPlaylist->get_out());
		}
		catch (std::exception& e)
		{
			delete pkCommand;
			throw;
		}
		delete pkCommand;
	}

	void insert_clip(iola::model::iclip* clipitem)
	{
		if (!clipitem)
			throw std::invalid_argument("Clip passed as parameter is NULL");

		assert(m_pkPlaylist);
		insert_clip_command* pkCommand = new insert_clip_command(m_pkPlaylist);
		try
		{
			clipitem->file()->set_pathurl_on(pkCommand);
			clipitem->markers()->update_markers_on(pkCommand);
			clipitem->video_settings()->update_settings_on(pkCommand);
			pkCommand->execute();
			on_producer_change_signal(m_pkPlaylist->get_in(), m_pkPlaylist->get_out());
		}
		catch (std::exception& e)
		{
			delete pkCommand;
			throw;
		}
		delete pkCommand;
	}

	void goto_previous_edit_on(iola::model::iplayhead* object)
	{
		const int frame = m_pkPlaylist->clip(mlt_whence_relative_current, -1);
		object->set_frame(frame);
	}

	void goto_next_edit_on(iola::model::iplayhead* object) 
	{
		const int frame = m_pkPlaylist->clip(mlt_whence_relative_current, 1);
		object->set_frame(frame);
	}

	///////////////////////////
	// iconsumable

	void connect_to(Mlt::Consumer* consumer)
	{
		if (!consumer)
			throw std::invalid_argument("Consumer passed as parameter is NULL");

		consumer->lock();
		consumer->connect(*m_pkPlaylist);
		consumer->unlock();
	}

	///////////////////////////
	// iasset

	void set_name(const std::string name)
	{
		m_strName = name;
	}

	void set_uuid(const std::string uuid)
	{
		m_strUUID = uuid;
	}

	void set_duration(const int duration)
	{
		m_iDuration = duration;
	}

	void set_timecode(const std::string timecode)
	{
		m_strTimecode = timecode;
	}

	iola::model::imarkers* markers()
	{
		assert(m_pkMarkers);
		return m_pkMarkers;
	}

	iola::model::ivideo_settings* video_settings()
	{
		assert(m_pkVideoSettings);
		return m_pkVideoSettings;
	}

	iola::model::iaudio_settings* audio_settings()
	{
		assert(m_pkAudioSettings);
		return m_pkAudioSettings;
	}

	iola::model::iplayhead* playhead()
	{
		return this;
	}

	void set_start_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int frame = m_pkPlaylist->get_in();
		m_pkPlaylist->unlock();
		//NOTE The sequence sets the start point here (cf. clip)
		//     in this implementation of the interface method.
		markers->set_start_point(frame);
	}

	void set_end_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int frame = m_pkPlaylist->get_out();
		m_pkPlaylist->unlock();
		//NOTE The sequence sets the end point here (cf. clip)
		//     in this implementation of the interface method.
		markers->set_end_point(frame);
	}

	void goto_start_on(iola::model::iplayhead* playhead)
	{
		if (!playhead)
			throw std::invalid_argument("Playhead passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int frame = m_pkPlaylist->get_in();
		m_pkPlaylist->unlock();
		playhead->set_frame(frame);
	}

	void goto_end_on(iola::model::iplayhead* playhead)
	{
		if (!playhead)
			throw std::invalid_argument("Playhead passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int frame = m_pkPlaylist->get_out();
		m_pkPlaylist->unlock();
		playhead->set_frame(frame);
	}

	///////////////////////////
	// iplayhead

	void set_in_point_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		int frame = m_pkPlaylist->frame();
		m_pkPlaylist->unlock();
		markers->set_in_point(frame);
	}

	void set_out_point_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		int frame = m_pkPlaylist->frame();
		m_pkPlaylist->unlock();
		markers->set_out_point(frame);
	}

	void set_start_point_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameter is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		int frame = m_pkPlaylist->frame();
		m_pkPlaylist->unlock();
		markers->set_start_point(frame);
	}

	void set_end_point_on(iola::model::imarkers* markers)
	{
		if (!markers)
			throw std::invalid_argument("Markers passed as parameters is NULL");

		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		int frame = m_pkPlaylist->frame();
		m_pkPlaylist->unlock();
		markers->set_end_point(frame);
	}

	void set_frame(const int frame)
	{
		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		m_pkPlaylist->seek(frame < 0 
				   ? 0 : frame > m_pkPlaylist->get_out() 
				   ? m_pkPlaylist->get_out() : frame);
		m_pkPlaylist->unlock();
	}

	void set_relative_frame(const int relative_frame)
	{
		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const int iFrame = m_pkPlaylist->frame() + relative_frame;
		m_pkPlaylist->seek(iFrame < 0 
				   ? 0 : iFrame > m_pkPlaylist->get_out() 
				   ? m_pkPlaylist->get_out() : iFrame);
		m_pkPlaylist->unlock();
	}

	void set_speed(const double speed)
	{
		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		m_pkPlaylist->set_speed(speed);
		m_pkPlaylist->unlock();
	}

	void set_relative_speed(const double relative_speed)
	{
		assert(m_pkPlaylist);
		m_pkPlaylist->lock();
		const double dSpeed = m_pkPlaylist->get_speed() + relative_speed;
		m_pkPlaylist->set_speed(dSpeed);
		m_pkPlaylist->unlock();
	}

private:
	std::string m_strName;
	std::string m_strUUID;
	int m_iDuration;
	std::string m_strTimecode;
	iola::model::imarkers* m_pkMarkers;
	iola::model::ivideo_settings* m_pkVideoSettings;
	iola::model::iaudio_settings* m_pkAudioSettings;
	Mlt::Playlist* m_pkPlaylist;
	Mlt::Profile m_kProfile;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_sequence

isequence* create_sequence()
{
	return new sequence_implementation();
}

} // namespace model
} // namespace iola
