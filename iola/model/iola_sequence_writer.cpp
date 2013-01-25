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
#include <sstream>

// BOOST
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

// RLOG
#include <rlog/rlog.h>

// IOLA
#include <iola/dom/anamorphic_element.h>
#include <iola/dom/depth_element.h>
#include <iola/dom/duration_element.h>
#include <iola/dom/element_factory.h>
#include <iola/dom/end_element.h>
#include <iola/dom/fielddominance_element.h>
#include <iola/dom/height_element.h>
#include <iola/dom/in_element.h>
#include <iola/dom/ivisitor.h>
#include <iola/dom/name_element.h>
#include <iola/dom/ntsc_element.h>
#include <iola/dom/out_element.h>
#include <iola/dom/pathurl_element.h>
#include <iola/dom/pixelaspectratio_element.h>
#include <iola/dom/root.h>
#include <iola/dom/samplerate_element.h>
#include <iola/dom/start_element.h>
#include <iola/dom/string_element.h>
#include <iola/dom/timebase_element.h>
#include <iola/dom/uuid_element.h>
#include <iola/dom/width_element.h>
#include <iola/model/iclip.h>
#include <iola/model/isequence_writer.h>
#include <iola/model/ivideo_settings.h>
#include <iola/xml/parser.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::iola_sequence_visitor

class iola_sequence_visitor :
	public iola::dom::ivisitor
{
public:
	iola_sequence_visitor()
	{
	}

	~iola_sequence_visitor()
	{
	}

	///////////////////////////
	// ivisitor

	void visit(iola::dom::anamorphic_element* element)
	{
		element->set(m_bAnamorphic);
	}

	void visit(iola::dom::depth_element* element)
	{
		element->set(m_iSampleDepth);
	}

	void visit(iola::dom::duration_element* element)
	{
		element->set(m_iDuration);
	}

	void visit(iola::dom::end_element* element)
	{
		// part of clipitem
	}

	void visit(iola::dom::fielddominance_element* element)
	{
		element->set(m_iFieldDominance);
	}

	void visit(iola::dom::height_element* element)
	{
		element->set(m_iHeight);
	}

	void visit(iola::dom::in_element* element)
	{
		// part of clipitem
	}

	void visit(iola::dom::name_element* element)
	{
		element->set(m_strName);
	}

	void visit(iola::dom::ntsc_element* element)
	{
		element->set(m_bNTSC);
	}

	void visit(iola::dom::out_element* element)
	{
		// part of clipitem
	}

	void visit(iola::dom::pathurl_element* element)
	{
		// part of clipitem
	}

	void visit(iola::dom::pixelaspectratio_element* element)
	{
		element->set(m_iPAR);
	}

	void visit(iola::dom::samplerate_element* element)
	{
		element->set(m_iSampleRate);
	}

	void visit(iola::dom::start_element* element)
	{
		// part of clipitem
	}

	void visit(iola::dom::string_element* element)
	{
		element->set(m_strTimecode);
	}

	void visit(iola::dom::timebase_element* element)
	{
		element->set(m_iTimebase);
	}
	
	void visit(iola::dom::uuid_element* element)
	{
		element->set(m_strUUID);
	}

	void visit(iola::dom::width_element* element)
	{
		element->set(m_iWidth);
	}

	///////////////////////////
	// isequence_writer

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

	void append_clipitem(iola::model::iclip* clipitem) //FIXME Implement!
	{
	}

	///////////////////////////
	// ivideo_settings

	void set_width(const int width)
	{
		m_iWidth = width;
	}

	void set_height(const int height)
	{
		m_iHeight = height;
	}

	void set_par(const iola::model::ivideo_settings::par_t par)
	{
		m_iPAR = par;
	}

	void set_anamorphic(const bool anamorphic)
	{
		m_bAnamorphic = anamorphic;
	}

	void set_field_dominance(const iola::model::ivideo_settings::field_t field)
	{
		m_iFieldDominance = field;
	}

	void set_fps_timebase(const int timebase)
	{
		m_iTimebase = timebase;
	}

	void set_fps_ntsc(const bool ntsc)
	{
		m_bNTSC = ntsc;
	}

	///////////////////////////
	// iaudio_settings

	void set_sample_depth(const int depth)
	{
		m_iSampleDepth = depth;
	}

	void set_sample_rate(const int rate)
	{
		m_iSampleRate = rate;
	}

private:
	std::string m_strName;
	std::string m_strUUID;
	int m_iDuration;
	std::string m_strTimecode;
	int m_iWidth;
	int m_iHeight;
	iola::model::ivideo_settings::par_t m_iPAR;
	bool m_bAnamorphic;
	iola::model::ivideo_settings::field_t m_iFieldDominance;
	int m_iTimebase;
	bool m_bNTSC;
	int m_iSampleDepth;
	int m_iSampleRate;
};

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::iola_sequence_writer_implementation

class iola_sequence_writer_implementation :
	public iola::model::isequence_writer
{
public:
	iola_sequence_writer_implementation(const boost::filesystem::path file) :
		m_pkVisitor(new iola_sequence_visitor()),
		m_kFile(file)
	{
		assert(m_pkVisitor);
	}

	~iola_sequence_writer_implementation()
	{
		assert(m_pkVisitor);
		delete m_pkVisitor;
	}

	void execute()
	{
		rDebug("%s: Save program as %s", __PRETTY_FUNCTION__, m_kFile.string().c_str());
		iola::dom::element_factory* pkFactory = new iola::dom::element_factory();
		iola::dom::root* pkRoot = new iola::dom::root();

		// This is the easiest way to create a skeleton
		std::stringstream isXML;
		isXML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		isXML << "<!DOCTYPE iola>";
		isXML << "<iola version=\"0\">";
		isXML << "  <sequence>";
		isXML << "    <name>Sequence</name>";
		isXML << "    <duration>0</duration>";
		isXML << "    <rate>";
		isXML << "      <timebase>30</timebase>";
		isXML << "      <ntsc>TRUE</ntsc>";
		isXML << "    </rate>";
		isXML << "    <in>-1</in>";
		isXML << "    <out>-1</out>";
		isXML << "    <timecode>";
		isXML << "      <string>00:00:00;00</string>";
		isXML << "    </timecode>";
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
		try
		{
			iola::xml::parser(pkFactory, pkRoot, isXML);
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
			delete pkFactory;
			delete pkRoot;
			return;
		}

		// Then call store to populate it
		pkRoot->store(m_pkVisitor);

		// Output it as XML
		boost::filesystem::ofstream osXML(m_kFile);
		pkRoot->xml(osXML);

		delete pkFactory;
		delete pkRoot;
	}

	///////////////////////////
	// isequence_writer

	void set_name(const std::string name)
	{
		m_pkVisitor->set_name(name);
	}

	void set_uuid(const std::string uuid)
	{
		m_pkVisitor->set_uuid(uuid);
	}

	void set_duration(const int duration)
	{
		m_pkVisitor->set_duration(duration);
	}

	void set_timecode(const std::string timecode)
	{
		m_pkVisitor->set_timecode(timecode);
	}

	void append_clipitem(iola::model::iclip* clipitem)
	{
		m_pkVisitor->append_clipitem(clipitem);
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
		m_pkVisitor->set_width(width);
	}

	void set_height(const int height)
	{
		m_pkVisitor->set_height(height);
	}

	void set_par(const iola::model::ivideo_settings::par_t par)
	{
		m_pkVisitor->set_par(par);
	}

	void set_anamorphic(const bool anamorphic)
	{
		m_pkVisitor->set_anamorphic(anamorphic);
	}

	void set_field_dominance(const iola::model::ivideo_settings::field_t field)
	{
		m_pkVisitor->set_field_dominance(field);
	}

	void set_fps_timebase(const int timebase)
	{
		m_pkVisitor->set_fps_timebase(timebase);
	}

	void set_fps_ntsc(const bool ntsc)
	{
		m_pkVisitor->set_fps_ntsc(ntsc);
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
		m_pkVisitor->set_sample_depth(depth);
	}

	void set_sample_rate(const int rate)
	{
		m_pkVisitor->set_sample_rate(rate);
	}

private:
	iola_sequence_visitor* m_pkVisitor;
	const boost::filesystem::path m_kFile;
};


} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_iola_sequence_writer

isequence_writer* create_iola_sequence_writer(const boost::filesystem::path file)
{
	return new iola_sequence_writer_implementation(file);
}

} // namespace model
} // namespace iola
