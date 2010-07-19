/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_TIMECODE_H
#define IOLA_MODEL_TIMECODE_H

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
#include <string>

namespace iola
{
namespace model
{

class timecode
{
public:
	timecode(int iTimebase);
	timecode(int iTimebase, int iFrameCount);
	timecode(int iTimebase, std::string strTimecode);
	~timecode();

	void set_timebase(int iTimebase);
	int get_timebase();

	void set_framecount(int iFramesCount);
	int get_framecount();

	void set_timecode(std::string strTimecode);
	std::string get_timecode(bool bDropFrame);

private:
	int m_iTimebase;
	int m_iFrameCount;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_TIMECODE_H