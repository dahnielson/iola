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

#include "element_factory.h"

#include "anamorphic_element.h"
#include "audio_element.h"
#include "clipitem_element.h"
#include "depth_element.h"
#include "duration_element.h"
#include "end_element.h"
#include "fielddominance_element.h"
#include "height_element.h"
#include "in_element.h"
#include "iola_element.h"
#include "media_element.h"
#include "name_element.h"
#include "ntsc_element.h"
#include "out_element.h"
#include "pathurl_element.h"
#include "pixelaspectratio_element.h"
#include "rate_element.h"
#include "samplecharacteristics_element.h"
#include "samplerate_element.h"
#include "sequence_element.h"
#include "start_element.h"
#include "string_element.h"
#include "timebase_element.h"
#include "timecode_element.h"
#include "track_element.h"
#include "video_element.h"
#include "width_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::element_factory

iola::xml::ielement* 
element_factory::factory(const std::string strElement)
{
	if (strElement == "anamorphic")
		return new anamorphic_element(strElement);
	else if (strElement == "audio")
		return new audio_element(strElement);
	else if (strElement == "depth")
		return new depth_element(strElement);
	else if (strElement == "clipitem")
		return new clipitem_element(strElement);
	else if (strElement == "duration")
		return new duration_element(strElement);
	else if (strElement == "end")
		return new end_element(strElement);
	else if (strElement == "fielddominance")
		return new fielddominance_element(strElement);
	else if (strElement == "height")
		return new height_element(strElement);
	else if (strElement == "in")
		return new in_element(strElement);
	else if (strElement == "iola")
		return new iola_element(strElement);
	else if (strElement == "media")
		return new media_element(strElement);
	else if (strElement == "name")
		return new name_element(strElement);
	else if (strElement == "ntsc")
		return new ntsc_element(strElement);
	else if (strElement == "out")
		return new out_element(strElement);
	else if (strElement == "pathurl")
		return new pathurl_element(strElement);
	else if (strElement == "pixelaspectratio")
		return new pixelaspectratio_element(strElement);
	else if (strElement == "rate")
		return new rate_element(strElement);
	else if (strElement == "samplecharacteristics")
		return new samplecharacteristics_element(strElement);
	else if (strElement == "samplerate")
		return new samplerate_element(strElement);
	else if (strElement == "sequence")
		return new sequence_element(strElement);
	else if (strElement == "start")
		return new start_element(strElement);
	else if (strElement == "string")
		return new string_element(strElement);
	else if (strElement == "timebase")
		return new timebase_element(strElement);
	else if (strElement == "timecode")
		return new timecode_element(strElement);
	else if (strElement == "track")
		return new track_element(strElement);
	else if (strElement == "video")
		return new video_element(strElement);
	else if (strElement == "width")
		return new width_element(strElement);
	else
		return 0;
}

} // namespace dom
} // namespace iola
