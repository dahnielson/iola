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

#include "audio_element.h"
#include "clipitem_element.h"
#include "duration_element.h"
#include "end_element.h"
#include "in_element.h"
#include "iola_element.h"
#include "media_element.h"
#include "out_element.h"
#include "pathurl_element.h"
#include "sequence_element.h"
#include "start_element.h"
#include "track_element.h"
#include "video_element.h"

namespace iola
{
namespace dom
{

////////////////////////////////////////////////////////////////////////////
// class iola::dom::element_factory

iola::xml::ielement* 
element_factory::factory(const std::string strElement)
{
	if (strElement == "audio")
		return new audio_element(strElement);
	else if (strElement == "clipitem")
		return new clipitem_element(strElement);
	else if (strElement == "duration")
		return new duration_element(strElement);
	else if (strElement == "end")
		return new end_element(strElement);
	else if (strElement == "in")
		return new in_element(strElement);
	else if (strElement == "iola")
		return new iola_element(strElement);
	else if (strElement == "media")
		return new media_element(strElement);
	else if (strElement == "out")
		return new out_element(strElement);
	else if (strElement == "pathurl")
		return new pathurl_element(strElement);
	else if (strElement == "sequence")
		return new sequence_element(strElement);
	else if (strElement == "start")
		return new start_element(strElement);
	else if (strElement == "track")
		return new track_element(strElement);
	else if (strElement == "video")
		return new video_element(strElement);
	else
		return 0;
}

} // namespace dom
} // namespace iola
