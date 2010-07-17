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

// RLOG
#include <rlog/rlog.h>

// BOOST
#include <boost/filesystem/fstream.hpp>

// STD
#include <iostream>
#include <sstream>

// IOLA
#include <iola/dom/element_factory.h>
#include <iola/dom/root.h>
#include <iola/xml/parser.h>
#include "project.h"

namespace iola
{
namespace domain
{

////////////////////////////////////////////////////////////////////////////
// class iola::domain::project

project::project() :
	m_pkSource(new Mlt::Playlist),
	m_pkProgram(new Mlt::Playlist),
	m_kProfile(Mlt::Profile()),
	m_iWidth(720),
	m_iHeight(480),
	m_iPAR(iola::domain::iproject::NTSC_601),
	m_bAnamorphic(false),
	m_iFieldDominance(iola::domain::iproject::EVEN),
	m_iTimebase(30),
	m_bNTSC(true),
	m_iSampleDepth(16),
	m_iSampleRate(48000),
	m_pkSourceProducerChangeEvent(0),
	m_pkProgramProducerChangeEvent(0)
{
	m_pkSourceProducerChangeEvent = m_pkSource->listen(
		"producer-changed", this, (mlt_listener)source_producer_change_callback
		);

	m_pkProgramProducerChangeEvent = m_pkProgram->listen(
		"producer-changed", this, (mlt_listener)program_producer_change_callback
		);

	source_new();
	program_new();
}

project::~project()
{
	delete m_pkSource;
	delete m_pkProgram;
	delete m_pkSourceProducerChangeEvent;
	delete m_pkProgramProducerChangeEvent;
}

///////////////////////////////////////////
// Profile

Mlt::Profile& project::get_profile()
{
	rDebug("%s: Assembling MLT profile", __PRETTY_FUNCTION__);

	// Storage Aspect Ratio
	m_kProfile.get_profile()->width = m_iWidth;
	m_kProfile.get_profile()->height = m_iHeight;

	// Display Aspect Ratio
	m_kProfile.get_profile()->display_aspect_num = get_dar_num();
	m_kProfile.get_profile()->display_aspect_den = get_dar_den();

	// Pixel Aspect Ratio
	m_kProfile.get_profile()->sample_aspect_num = get_par_num();
	m_kProfile.get_profile()->sample_aspect_den = get_par_den();

	// Progressive
	m_kProfile.get_profile()->progressive = get_progressive();

	// Frame Rate
	m_kProfile.get_profile()->frame_rate_num = get_fps_num();
	m_kProfile.get_profile()->frame_rate_den = get_fps_den();

	return m_kProfile;
}

void project::set_width(int width)
{
	m_iWidth = width;
	on_sar_change_signal();
}

int project::get_width()
{
	return m_iWidth;
}

void project::set_height(int height)
{
	m_iHeight = height;
	on_sar_change_signal();
}

int project::get_height()
{
	return m_iHeight;
}

int project::get_dar_num()
{
	int iDARNum;

	if (m_iWidth == 720 && m_iHeight == 480)
	{
		if (m_bAnamorphic)
			iDARNum = 16;
		else
			iDARNum = 4;
	}
	else if (m_iWidth == 720 && m_iHeight == 576)
	{
		if (m_bAnamorphic)
			iDARNum = 16;
		else
			iDARNum = 4;
	}
	else if (m_iWidth == 960 && m_iHeight == 720)
		iDARNum = 16;
	else if (m_iWidth == 1280 && m_iHeight == 720)
		iDARNum = 16;
	else if (m_iWidth == 1280 && m_iHeight == 1080)
		iDARNum = 16;
	else if (m_iWidth == 1440 && m_iHeight == 1080)
		iDARNum = 16;
	else if (m_iWidth == 1920 && m_iHeight == 1080)
		iDARNum = 16;
	else
		iDARNum = m_iWidth;

	return iDARNum;
}

int project::get_dar_den()
{
	int iDARDen;

	if (m_iWidth == 720 && m_iHeight == 480)
	{
		if (m_bAnamorphic)
			iDARDen = 9;
		else
			iDARDen = 3;
	}
	else if (m_iWidth == 720 && m_iHeight == 576)
	{
		if (m_bAnamorphic)
			iDARDen = 9;
		else
			iDARDen = 3;
	}
	else if (m_iWidth == 960 && m_iHeight == 720)
		iDARDen = 9;
	else if (m_iWidth == 1280 && m_iHeight == 720)
		iDARDen = 9;
	else if (m_iWidth == 1280 && m_iHeight == 1080)
		iDARDen = 9;
	else if (m_iWidth == 1440 && m_iHeight == 1080)
		iDARDen = 9;
	else if (m_iWidth == 1920 && m_iHeight == 1080)
		iDARDen = 9;
	else
		iDARDen = m_iHeight;

	return iDARDen;
}

void project::set_par(iola::domain::iproject::par_t par)
{
	m_iPAR = par;
	on_par_change_signal();
}

iola::domain::iproject::par_t project::get_par()
{
	return m_iPAR;
}

int project::get_par_num()
{
	int iPARNum;

	switch (m_iPAR)
	{
	case SQUARE:
		iPARNum = 1;
		break;
	case NTSC_601:
		iPARNum = 8;
		if (m_bAnamorphic)
			iPARNum *= 4;
		break;
	case PAL_601:
		iPARNum = 16;
		if (m_bAnamorphic)
			iPARNum *= 4;
		break;
	case HD_1280x1080:
		iPARNum = 3;
		break;
	case HD_960x720:
	case HD_1440x1080:
		iPARNum = 4;
		break;
	};

	return iPARNum;
}

int project::get_par_den()
{
	int iPARDen;

	switch (m_iPAR)
	{
	case SQUARE:
		iPARDen = 1;
		break;
	case NTSC_601:
		iPARDen = 9;
		if (m_bAnamorphic)
			iPARDen *= 3;
		break;
	case PAL_601:
		iPARDen = 15;
		if (m_bAnamorphic)
			iPARDen *= 3;
		break;
	case HD_1280x1080:
		iPARDen = 2;
		break;
	case HD_960x720:
	case HD_1440x1080:
		iPARDen = 3;
		break;
	};

	return iPARDen;
}

void project::set_anamorphic(bool anamorphic)
{
	m_bAnamorphic = anamorphic;
	on_dar_change_signal();
	on_par_change_signal();
}

bool project::get_anamorphic()
{
	return m_bAnamorphic;
}

void project::set_field_dominance(iola::domain::iproject::field_t dominance)
{
	m_iFieldDominance = dominance;
	on_field_change_signal();
}

iola::domain::iproject::field_t project::get_field_dominance()
{
	return m_iFieldDominance;
}

bool project::get_progressive()
{
	switch (m_iFieldDominance)
	{
	case NONE:
		return true;
	case EVEN:
	case ODD:
		return false;
	};
}

void project::set_fps_timebase(int timebase)
{
	m_iTimebase = timebase;
	on_fps_change_signal();
}

int project::get_fps_timebase()
{
	return m_iTimebase;
}

void project::set_fps_ntsc(bool ntsc)
{
	m_bNTSC = ntsc;
	on_fps_change_signal();
}

bool project::get_fps_ntsc()
{
	return m_bNTSC;
}

int project::get_fps_num()
{
	return m_bNTSC ? m_iTimebase * 1000 : m_iTimebase;
}

int project::get_fps_den()
{
	return m_bNTSC ? 1001 : 1;
}

void project::set_sample_depth(int depth)
{
	m_iSampleDepth = depth;
	on_sample_change_signal();
}

int project::get_sample_depth()
{
	return m_iSampleDepth;
}

void project::set_sample_rate(int rate)
{
	m_iSampleRate = rate;
	on_sample_change_signal();
}

int project::get_sample_rate()
{
	return m_iSampleRate;
}

///////////////////////////////////////////
// Source

void project::source_connect_consumer(Mlt::Consumer* consumer)
{
	consumer->lock();
	consumer->connect(*m_pkSource);
	consumer->unlock();
}

void project::source_new()
{
	rDebug("%s: Create a new source", __PRETTY_FUNCTION__);
	m_pkSource->clear();
	m_pkSource->set_speed(0);
	m_pkSource->set("meta.iola.mark_in", -1);
	m_pkSource->set("meta.iola.mark_out", -1);
	on_source_load_signal();
	on_source_marks_change_signal();
}

void project::source_load(boost::filesystem::path clip)
{
	rDebug("%s: Load %s as source", __PRETTY_FUNCTION__, clip.string().c_str());
	m_pkSource->lock();
	m_pkSource->clear();
	Mlt::Producer* pkClipSource = new Mlt::Producer(get_profile(), clip.string().c_str());
	m_pkSource->append(*pkClipSource);
	m_pkSource->set_speed(0);
	m_pkSource->seek(0);
	m_pkSource->set("meta.iola.mark_in", -1);
	m_pkSource->set("meta.iola.mark_out", -1);
	m_pkSource->unlock();
	on_source_load_signal();
	on_source_marks_change_signal();
}

int project::source_get_start()
{
	return m_pkSource->get_in();
}

int project::source_get_end()
{
	return m_pkSource->get_out();
}

double project::source_get_speed()
{
	if (!m_pkSource)
		return 0;

	m_pkSource->lock();
	double speed = m_pkSource->get_speed();
	m_pkSource->unlock();
	return speed;
}

void project::source_set_speed(double speed)
{
	if (m_pkSource && m_pkSource->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
		m_pkSource->lock();
		m_pkSource->set_speed(speed);
		m_pkSource->unlock();
	}
	on_source_playback_signal();
}

void project::source_seek(int position)
{
	if (m_pkSource)
	{
		rDebug("%s: Seek position at %i", __PRETTY_FUNCTION__, position);
		m_pkSource->lock();
		m_pkSource->seek(position < 0 ? 0 : position > m_pkSource->get_out() ? m_pkSource->get_out() : position);
		m_pkSource->unlock();
	}
	on_source_seek_signal();
}

void project::source_step_forward()
{
	if (!m_pkSource)
		return;

	source_set_speed(0);
	int position = m_pkSource->frame();
	source_seek(++position);
}

void project::source_step_backward()
{
	if (!m_pkSource)
		return;

	source_set_speed(0);
	int position = m_pkSource->frame();
	source_seek(--position);
}

void project::source_play_forward()
{
	if (!m_pkSource)
		return;

	double speed = m_pkSource->get_speed();
	if (speed == 0 || speed + 1 == 0)
		speed = 1;
	else if (speed < 0)
		speed /= 2;
	else if (speed < 8)
		speed *= 2;
	source_set_speed(speed);
}

void project::source_play_reverse()
{
	if (!m_pkSource)
		return;

	double speed = m_pkSource->get_speed();
	if (speed == 0 || speed - 1 == 0)
		speed = -1;
	else if (speed > 0)
		speed /= 2;
	else if (speed > -8)
		speed *= 2;
	source_set_speed(speed);
}

void project::source_pause()
{
	source_set_speed(0);
}

int project::source_get_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int mark = m_pkSource->get_int("meta.iola.mark_in");
		m_pkSource->unlock();
		return mark;
	}
}

int project::source_get_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int mark = m_pkSource->get_int("meta.iola.mark_out");
		m_pkSource->unlock();
		return mark;
	}
}

void project::source_set_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Set source mark in at %i", __PRETTY_FUNCTION__, m_pkSource->frame());
		m_pkSource->set("meta.iola.mark_in", m_pkSource->frame());
		m_pkSource->unlock();
		on_source_marks_change_signal();
	}
}

void project::source_set_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Set source mark out at %i", __PRETTY_FUNCTION__, m_pkSource->frame());
		m_pkSource->set("meta.iola.mark_out", m_pkSource->frame());
		m_pkSource->unlock();
		on_source_marks_change_signal();
	}
}

void project::source_set_mark_clip()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		const int in = m_pkSource->get_in();
		const int out = m_pkSource->get_out();
		rDebug("%s: Set source mark in at %i and out at %i", __PRETTY_FUNCTION__, in, out);
		m_pkSource->set("meta.iola.mark_in", in);
		m_pkSource->set("meta.iola.mark_out", out);
		m_pkSource->unlock();
		on_source_marks_change_signal();
	}
}

void project::source_clear_mark_in()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Clear source mark in", __PRETTY_FUNCTION__);
		m_pkSource->set("meta.iola.mark_in", -1);
		m_pkSource->unlock();
		on_source_marks_change_signal();
	}
}

void project::source_clear_mark_out()
{
	if (m_pkSource)
	{
		m_pkSource->lock();
		rDebug("%s: Clear source mark out", __PRETTY_FUNCTION__);
		m_pkSource->set("meta.iola.mark_out", -1);
		m_pkSource->unlock();
		on_source_marks_change_signal();
	}
}

void project::source_goto_mark_in()
{
	if (m_pkSource)
	{
		const int mark = source_get_mark_in();
		rDebug("%s: Goto source mark in at %i", __PRETTY_FUNCTION__, mark);
		source_seek(mark);
	}
}

void project::source_goto_mark_out()
{
	if (m_pkSource)
	{
		const int mark = source_get_mark_out();
		rDebug("%s: Goto source mark out at %i", __PRETTY_FUNCTION__, mark);
		source_seek(mark);
	}
}

void project::source_goto_start()
{
	if (m_pkSource)
	{
		const int frame = m_pkSource->get_in();
		rDebug("%s: Goto source start at %i", __PRETTY_FUNCTION__, frame);
		source_seek(frame);
	}
}

void project::source_goto_end()
{
	if (m_pkSource)
	{
		const int frame = m_pkSource->get_out();
		rDebug("%s: Goto source end at %i", __PRETTY_FUNCTION__, frame);
		source_seek(frame);
	}
}

///////////////////////////////////////////
// Program

void project::program_connect_consumer(Mlt::Consumer* consumer)
{
	consumer->lock();
	consumer->connect(*m_pkProgram);
	consumer->unlock();
}

void project::program_new()
{
	if (!m_pkProgram)
		rError("%s: No program!", __PRETTY_FUNCTION__);

	rDebug("%s: Create a new program", __PRETTY_FUNCTION__);
	m_pkProgram->clear();
	m_pkProgram->set_speed(0);
	m_pkProgram->set("meta.iola.mark_in", -1);
	m_pkProgram->set("meta.iola.mark_out", -1);
	on_program_load_signal();
	on_program_marks_change_signal();
}

void project::program_load(boost::filesystem::path sequence)
{
	if (!m_pkProgram)
		rError("%s: No program!", __PRETTY_FUNCTION__);

	rDebug("%s: Load %s as program", __PRETTY_FUNCTION__, sequence.string().c_str());
	iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
	iola::dom::root* pkRoot = new iola::dom::root();
	boost::filesystem::ifstream isXML(sequence);
	iola::xml::parser(pkFactory, pkRoot, isXML);
	m_pkProgram->clear();
	m_pkProgram->set_speed(0);
	m_pkProgram->set("meta.iola.mark_in", -1);
	m_pkProgram->set("meta.iola.mark_out", -1);
	pkRoot->restore();
	delete pkFactory;
        delete pkRoot;
	on_program_load_signal();
	on_program_marks_change_signal();
}

void project::program_save(boost::filesystem::path sequence)
{
	if (!m_pkProgram)
		return;

	rDebug("%s: Save program as %s", __PRETTY_FUNCTION__, sequence.string().c_str());
	iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
	iola::dom::root* pkRoot = new iola::dom::root();

	// This is the easiest way to create a skeleton
	std::stringstream isXML;
	isXML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	isXML << "<!DOCTYPE iola>";
	isXML << "<iola version=\"0\">";
	isXML << "  <sequence>";
	isXML << "    <duration>0</duration>";
	isXML << "    <rate>";
	isXML << "      <timebase>30</timebase>";
	isXML << "      <ntsc>TRUE</ntsc>";
	isXML << "    </rate>";
	isXML << "    <media>";
	isXML << "      <video>";
	isXML << "        <samplecharacteristics>";
	isXML << "          <width>720</width>";
	isXML << "          <height>480</height>";
	isXML << "          <anamorphic>FALSE</anamorphic>";
	isXML << "          <pixelaspectratio>NTSC-601</pixelaspectratio>";
	isXML << "          <fielddominance>LOWER</fielddominance>";
	isXML << "        </samplecharacteristics>";
	isXML << "        <track></track>";
	isXML << "      </video>";
	isXML << "      <audio>";
	isXML << "        <samplecharacteristics>";
	isXML << "          <depth>16</depth>";
	isXML << "          <samplerate>48000</samplerate>";
	isXML << "        </samplecharacteristics>";
	isXML << "      </audio>";
	isXML << "    </media>";
	isXML << "  </sequence>";
	isXML << "</iola>";

	// Parse the skeleton into an object
	iola::xml::parser(pkFactory, pkRoot, isXML);

	// Then call store to populate it
	pkRoot->store();

	// Output it as XML
	boost::filesystem::ofstream osXML(sequence);
	pkRoot->xml(osXML);

	delete pkFactory;
	delete pkRoot;
}

int project::program_get_start()
{
	return m_pkProgram->get_in();
}

int project::program_get_end()
{
	return m_pkProgram->get_out();
}

void project::program_set_duration(int duration)
{
	if (m_pkProgram && m_pkProgram->get_length() != duration)
	{
		rDebug("%s: Set duration to %i", __PRETTY_FUNCTION__, duration);
		if (m_pkProgram->get_length() > duration)
		{
			// Make shorter
			const int delta = m_pkProgram->get_length() - duration;
			m_pkProgram->remove_region(duration, delta);
		}
		else
		{
			// Make longer
			m_pkProgram->lock();
			m_pkProgram->blank(duration);
			m_pkProgram->unlock();
		}
	}
}

int project::program_get_duration()
{
	if (!m_pkProgram)
		return 0;

	m_pkProgram->lock();
	int duration = m_pkProgram->get_length();
	m_pkProgram->unlock();
	return duration;
}

double project::program_get_speed()
{
	if (!m_pkProgram)
		return 0;

	m_pkProgram->lock();
	double speed = m_pkProgram->get_speed();
	m_pkProgram->unlock();
	return speed;
}

void project::program_set_speed(double speed)
{
	if (m_pkProgram && m_pkProgram->get_speed() != speed)
	{
		rDebug("%s: Set speed x%f", __PRETTY_FUNCTION__, speed);
		m_pkProgram->lock();
		m_pkProgram->set_speed(speed);
		m_pkProgram->unlock();
	}
	on_program_playback_signal();
}

void project::program_seek(int position)
{
	if (m_pkProgram)
	{
		rDebug("%s: Seek position at %i", __PRETTY_FUNCTION__, position);
		m_pkProgram->lock();
		m_pkProgram->seek(position < 0 ? 0 : position > m_pkProgram->get_out() ? m_pkProgram->get_out() : position);
		m_pkProgram->unlock();
	}
	on_program_seek_signal();
}

int project::program_get_clip_count()
{
	if (m_pkProgram)
		return m_pkProgram->count();
}

Mlt::ClipInfo* project::program_get_clip_info(int index)
{
	if (m_pkProgram)
		return m_pkProgram->clip_info(index);
}

void project::program_step_forward()
{
	if (!m_pkProgram)
		return;

	program_set_speed(0);
	int position = m_pkProgram->frame();
	program_seek(++position);
}

void project::program_step_backward()
{
	if (!m_pkProgram)
		return;

	program_set_speed(0);
	int position = m_pkProgram->frame();
	program_seek(--position);
}

void project::program_play_forward()
{
	if (!m_pkProgram)
		return;

	double speed = m_pkProgram->get_speed();
	if (speed == 0 || speed + 1 == 0)
		speed = 1;
	else if (speed < 0)
		speed /= 2;
	else if (speed < 8)
		speed *= 2;
	program_set_speed(speed);
}

void project::program_play_reverse()
{
	if (!m_pkProgram)
		return;

	double speed = m_pkProgram->get_speed();
	if (speed == 0 || speed - 1 == 0)
		speed = -1;
	else if (speed > 0)
		speed /= 2;
	else if (speed > -8)
		speed *= 2;
	program_set_speed(speed);
}

void project::program_pause()
{
	program_set_speed(0);
}

int project::program_get_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int mark = m_pkProgram->get_int("meta.iola.mark_in");
		m_pkProgram->unlock();
		return mark;
	}
}

int project::program_get_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int mark = m_pkProgram->get_int("meta.iola.mark_out");
		m_pkProgram->unlock();
		return mark;
	}}

void project::program_set_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Set program mark in at %i", __PRETTY_FUNCTION__, m_pkProgram->frame());
		m_pkProgram->set("meta.iola.mark_in", m_pkProgram->frame());
		m_pkProgram->unlock();
		on_program_marks_change_signal();
	}
}

void project::program_set_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Set program mark out at %i", __PRETTY_FUNCTION__, m_pkProgram->frame());
		m_pkProgram->set("meta.iola.mark_out", m_pkProgram->frame());
		m_pkProgram->unlock();
		on_program_marks_change_signal();
	}
}

void project::program_set_mark_cut()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		const int in = m_pkProgram->clip(mlt_whence_relative_current, -1);
		const int out = m_pkProgram->clip(mlt_whence_relative_current, 1);
		rDebug("%s: Set program mark in at %i and out at %i", __PRETTY_FUNCTION__, in, out);
		m_pkProgram->set("meta.iola.mark_in", in);
		m_pkProgram->set("meta.iola.mark_out", out);
		m_pkProgram->unlock();
		on_program_marks_change_signal();
	}
}

void project::program_clear_mark_in()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Clear program mark in", __PRETTY_FUNCTION__);
		m_pkProgram->set("meta.iola.mark_in", -1);
		m_pkProgram->unlock();
		on_program_marks_change_signal();
	}
}

void project::program_clear_mark_out()
{
	if (m_pkProgram)
	{
		m_pkProgram->lock();
		rDebug("%s: Clear program mark out", __PRETTY_FUNCTION__);
		m_pkProgram->set("meta.iola.mark_out", -1);
		m_pkProgram->unlock();
		on_program_marks_change_signal();
	}
}

void project::program_goto_mark_in()
{
	if (m_pkProgram)
	{
		const int mark = program_get_mark_in();
		rDebug("%s: Goto program mark in at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void project::program_goto_mark_out()
{
	if (m_pkProgram)
	{
		const int mark = program_get_mark_out();
		rDebug("%s: Goto program mark out at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void project::program_goto_previous_edit()
{
	if (m_pkProgram)
	{
		const int mark = m_pkProgram->clip(mlt_whence_relative_current, -1);
		rDebug("%s: Goto previous edit at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void project::program_goto_next_edit()
{
	if (m_pkProgram)
	{
		const int mark = m_pkProgram->clip(mlt_whence_relative_current, 1);
		rDebug("%s: Goto next edit at %i", __PRETTY_FUNCTION__, mark);
		program_seek(mark);
	}
}

void project::program_goto_start()
{
	if (m_pkProgram)
	{
		const int frame = m_pkProgram->get_in();
		rDebug("%s: Goto program start at %i", __PRETTY_FUNCTION__, frame);
		program_seek(frame);
	}
}

void project::program_goto_end()
{
	if (m_pkProgram)
	{
		const int frame = m_pkProgram->get_out();
		rDebug("%s: Goto program end at %i", __PRETTY_FUNCTION__, frame);
		program_seek(frame);
	}
}

void project::program_insert()
{
	if (!m_pkProgram)
		rWarning("%s: m_pkProgram is NULL", __PRETTY_FUNCTION__);

	if (!m_pkSource)
		rWarning("%s: m_pkSource is NULL", __PRETTY_FUNCTION__);

	if (m_pkProgram && m_pkSource)
	{
		int program_in, source_in, source_out;

		if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
		    source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
		    program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Four-point editing: program in/out takes precedence", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() == -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source in marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() == -1 && source_get_mark_out() != -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source out marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_out() - duration;
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() == -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in and source in/out marked", __PRETTY_FUNCTION__);
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() == -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program out and source in/out marked", __PRETTY_FUNCTION__);
			const int duration = source_get_mark_out() - source_get_mark_in();
			program_in = program_get_mark_out() - duration;
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else
		{
			rDebug("%s: Not good enough in/out points marked to perform three-point edit", __PRETTY_FUNCTION__);
			on_alert("Not good enough in/out points marked to perform three-point edit");
			return;
		}

		Mlt::ClipInfo* pkInfo = m_pkSource->clip_info(0);
		program_insert(boost::filesystem::path(pkInfo->resource), program_in, source_in, source_out);
		if (program_in > 0)
			program_seek(program_in + 1);
		Mlt::Playlist::delete_clip_info(pkInfo);
	}
}

void project::program_insert(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out)
{
	if (m_pkProgram)
	{
		rDebug("%s: Insert edit into playlist with %i clips: program_in=%i source_in=%i source_out=%i resource=%s",
		       __PRETTY_FUNCTION__, m_pkProgram->count(), program_in, source_in, source_out, resource.string().c_str());

		m_pkProgram->lock();
		const int clip_index = m_pkProgram->get_clip_index_at(program_in);
		m_pkProgram->split(clip_index, program_in - m_pkProgram->clip_start(clip_index));
		Mlt::Producer* pkClipSource = new Mlt::Producer(get_profile(), resource.string().c_str());
		m_pkProgram->insert(*pkClipSource, clip_index+1, source_in, source_out);
		m_pkProgram->unlock();

		rDebug("%s: Playlist now contain %i clips", __PRETTY_FUNCTION__, m_pkProgram->count());
	}
}

void project::program_overwrite()
{
	if (!m_pkProgram)
		rWarning("%s: m_pkProgram is NULL", __PRETTY_FUNCTION__);

	if (!m_pkSource)
		rWarning("%s: m_pkSource is NULL", __PRETTY_FUNCTION__);

	if (m_pkProgram && m_pkSource)
	{
		int program_in, source_in, source_out;

		if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
		    source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
		    program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Four-point editing: program in/out takes precedence", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() == -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source in marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_in() + duration;
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() == -1 && source_get_mark_out() != -1 &&
			 program_get_mark_in() < program_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in/out and source out marked", __PRETTY_FUNCTION__);
			const int duration = program_get_mark_out() - program_get_mark_in();
			program_in = program_get_mark_in();
			source_in = source_get_mark_out() - duration;
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() != -1 && program_get_mark_out() == -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program in and source in/out marked", __PRETTY_FUNCTION__);
			program_in = program_get_mark_in();
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else if (program_get_mark_in() == -1 && program_get_mark_out() != -1 &&
			 source_get_mark_in() != -1 && source_get_mark_out() != -1 &&
			 source_get_mark_in() < source_get_mark_out())
		{
			rDebug("%s: Three-point editing: program out and source in/out marked", __PRETTY_FUNCTION__);
			const int duration = source_get_mark_out() - source_get_mark_in();
			program_in = program_get_mark_out() - duration;
			source_in = source_get_mark_in();
			source_out = source_get_mark_out();
		}
		else
		{
			rDebug("%s: Not good enough in/out points marked to perform three-point edit", __PRETTY_FUNCTION__);
			on_alert("Not good enough in/out points marked to perform three-point edit");
			return;
		}

		Mlt::ClipInfo* pkInfo = m_pkSource->clip_info(0);
		program_overwrite(boost::filesystem::path(pkInfo->resource), program_in, source_in, source_out);
		if (program_in > 0)
			program_seek(program_in + 1);
		Mlt::Playlist::delete_clip_info(pkInfo);
	}
}

void project::program_overwrite(boost::filesystem::path resource, const int program_in, const int source_in, const int source_out)
{
	if (m_pkProgram)
	{
		rDebug("%s: Overwrite edit into playlist with %i clips: program_in=%i source_in=%i source_out=%i resource=%s",
		       __PRETTY_FUNCTION__, m_pkProgram->count(), program_in, source_in, source_out, resource.string().c_str());

		m_pkProgram->lock();
		const int clip_index = m_pkProgram->remove_region(program_in, source_out - source_in);
		Mlt::Producer* pkClipSource = new Mlt::Producer(get_profile(), resource.string().c_str());
		m_pkProgram->insert(*pkClipSource, clip_index, source_in, source_out);
		m_pkProgram->unlock();

		rDebug("%s: Playlist now contain %i clips", __PRETTY_FUNCTION__, m_pkProgram->count());
	}
}

} // namespace domain
} // namespace iola
