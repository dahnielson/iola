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
#include <sstream>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// RLOG
#include <rlog/rlog.h>

// IOLA
#include <iola/dom/anamorphic_element.h>
#include <iola/dom/audio_element.h>
#include <iola/dom/clip_element.h>
#include <iola/dom/depth_element.h>
#include <iola/dom/duration_element.h>
#include <iola/dom/file_element.h>
#include <iola/dom/fielddominance_element.h>
#include <iola/dom/height_element.h>
#include <iola/dom/in_element.h>
#include <iola/dom/iola_element.h>
#include <iola/dom/media_element.h>
#include <iola/dom/name_element.h>
#include <iola/dom/ntsc_element.h>
#include <iola/dom/out_element.h>
#include <iola/dom/pathurl_element.h>
#include <iola/dom/pixelaspectratio_element.h>
#include <iola/dom/rate_element.h>
#include <iola/dom/root.h>
#include <iola/dom/samplecharacteristics_element.h>
#include <iola/dom/samplerate_element.h>
#include <iola/dom/string_element.h>
#include <iola/dom/timebase_element.h>
#include <iola/dom/timecode_element.h>
#include <iola/dom/uuid_element.h>
#include <iola/dom/video_element.h>
#include <iola/dom/width_element.h>
#include <iola/model/iclip.h>
#include <iola/model/ifile_characteristics.h>
#include <iola/model/imarker_characteristics.h>
#include <iola/model/iclip_writer.h>


namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::iola_clip_writer_implementation

class iola_clip_writer_implementation :
	public iola::model::iclip_writer
{
public:
	iola_clip_writer_implementation(const boost::filesystem::path file) :
		m_kFile(file),
		m_pkRoot(new iola::dom::root()),
		m_pkClip(new iola::dom::clip_element("sequence")),
		m_pkRate(new iola::dom::rate_element("rate")),
		m_pkVideoCharacteristics(new iola::dom::samplecharacteristics_element("samplecharacteristics")),
		m_pkAudioCharacteristics(new iola::dom::samplecharacteristics_element("samplecharacteristics"))
	{
		assert(m_pkRoot);
		assert(m_pkClip);
		assert(m_pkRate);
		assert(m_pkVideoCharacteristics);
		assert(m_pkAudioCharacteristics);

		iola::dom::iola_element* pkIola = new iola::dom::iola_element("iola");
		iola::dom::media_element* pkMedia = new iola::dom::media_element("media");
		iola::dom::video_element* pkVideo = new iola::dom::video_element("video");
		iola::dom::audio_element* pkAudio = new iola::dom::audio_element("audio");

		m_pkRoot->child(pkIola);
		pkIola->child(m_pkClip);
		m_pkClip->child(m_pkRate);
		m_pkClip->child(pkMedia);
		pkMedia->child(pkVideo);
		pkVideo->child(m_pkVideoCharacteristics);
		pkMedia->child(pkAudio);
		pkAudio->child(m_pkAudioCharacteristics);
	}

	~iola_clip_writer_implementation()
	{
		assert(m_pkRoot);
		delete m_pkRoot;
	}

	///////////////////////////
	// iclip_writer

	void execute()
	{
		rDebug("%s: Save clip as %s", __PRETTY_FUNCTION__, m_kFile.string().c_str());
		boost::filesystem::ofstream osXML(m_kFile);
		m_pkRoot->xml(osXML);
	}

	void set_name(const std::string name)
	{
		iola::dom::name_element* pkName = new iola::dom::name_element("name");
		pkName->set(name);
		m_pkClip->child(pkName);
	}

	void set_uuid(const std::string uuid)
	{
		iola::dom::uuid_element* pkUUID = new iola::dom::uuid_element("uuid");
		pkUUID->set(uuid);
		m_pkClip->child(pkUUID);
	}

	void set_duration(const int duration)
	{
		iola::dom::duration_element* pkDuration = new iola::dom::duration_element("duration");
		pkDuration->set(duration);
		m_pkClip->child(pkDuration);
	}

	void set_timecode(const std::string timecode)
	{
		iola::dom::timecode_element* pkTimecode = new iola::dom::timecode_element("timecode");
		iola::dom::string_element* pkString = new iola::dom::string_element("string");
		pkString->set(timecode);
		pkTimecode->child(pkString);
		m_pkClip->child(pkTimecode);
	}

	void set_file(const boost::filesystem::path file) //FIXME Implement
	{
	}

	///////////////////////////
	// ivideo_settings

	void update_settings_on(iola::model::ivideo_settings* object)
	{
		assert(!"Unimplemented");
	}

	void update_settings_on(iola::model::ivideo_characteristics* object)
	{
		assert(!"Unimplemented");
	}

	void set_width(const int width)
	{
		iola::dom::width_element* pkWidth = new iola::dom::width_element("width");
		pkWidth->set(width);
		m_pkVideoCharacteristics->child(pkWidth);
	}

	void set_height(const int height)
	{
		iola::dom::height_element* pkHeight = new iola::dom::height_element("height");
		pkHeight->set(height);
		m_pkVideoCharacteristics->child(pkHeight);
	}

	void set_par(const iola::model::ivideo_settings::par_t par)
	{
		iola::dom::pixelaspectratio_element* pkPAR = new iola::dom::pixelaspectratio_element("pixelaspectratio");
		pkPAR->set(par);
		m_pkVideoCharacteristics->child(pkPAR);
	}

	void set_anamorphic(const bool anamorphic)
	{
		iola::dom::anamorphic_element* pkAnamorphic =  new iola::dom::anamorphic_element("anamorphic");
		pkAnamorphic->set(anamorphic);
		m_pkVideoCharacteristics->child(pkAnamorphic);
	}

	void set_field_dominance(const iola::model::ivideo_settings::field_t field)
	{
		iola::dom::fielddominance_element* pkField = new iola::dom::fielddominance_element("fielddominance");
		pkField->set(field);
		m_pkVideoCharacteristics->child(pkField);
	}

	void set_fps_timebase(const int timebase)
	{
		iola::dom::timebase_element* pkTimebase = new iola::dom::timebase_element("timebase");
		pkTimebase->set(timebase);
		m_pkRate->child(pkTimebase);
	}

	void set_fps_ntsc(const bool ntsc)
	{
		iola::dom::ntsc_element* pkNTSC = new iola::dom::ntsc_element("ntsc");
		pkNTSC->set(ntsc);
		m_pkRate->child(pkNTSC);
	}

	///////////////////////////
	// iaudio_settings

	void update_settings_on(iola::model::iaudio_settings* object)
	{
		assert(!"Unimplemented");
	}

	void update_settings_on(iola::model::iaudio_characteristics* object)
	{
		assert(!"Unimplemented");
	}

	void set_sample_depth(const int depth)
	{
		iola::dom::depth_element* pkDepth = new iola::dom::depth_element("depth");
		pkDepth->set(depth);
		m_pkAudioCharacteristics->child(pkDepth);
	}

	void set_sample_rate(const int rate)
	{
		iola::dom::samplerate_element* pkRate = new iola::dom::samplerate_element("samplerate");
		pkRate->set(rate);
		m_pkAudioCharacteristics->child(pkRate);
	}

private:
	const boost::filesystem::path m_kFile;
	iola::dom::root* m_pkRoot;
	iola::dom::clip_element* m_pkClip;
	iola::dom::rate_element* m_pkRate;
	iola::dom::samplecharacteristics_element* m_pkVideoCharacteristics;
	iola::dom::samplecharacteristics_element* m_pkAudioCharacteristics;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_iola_clip_writer

iclip_writer* create_iola_clip_writer(const boost::filesystem::path file)
{
	return new iola_clip_writer_implementation(file);
}

} // namespace model
} // namespace iola
