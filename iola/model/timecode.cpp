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
#include <cmath>

// RLOG
#include <rlog/rlog.h>

// BOOST
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

// IOLA
#include "timecode.h"

namespace iola
{
namespace model
{

timecode::timecode(int iTimebase) :
	m_iFrameCount(0),
	m_iTimebase(iTimebase)
{
	pthread_mutex_init(&mutex, NULL);
}

timecode::timecode(int iTimebase, int iFrameCount) :
	m_iFrameCount(iFrameCount),
	m_iTimebase(iTimebase)
{
	pthread_mutex_init(&mutex, NULL);
}

timecode::timecode(int iTimebase, std::string strTimecode) :
	m_iTimebase(iTimebase)
{
	pthread_mutex_init(&mutex, NULL);
	set_timecode(strTimecode);
}

timecode::~timecode()
{
	pthread_mutex_destroy(&mutex);
}

void timecode::set_timebase(int iTimebase)
{
	pthread_mutex_lock(&mutex);
	m_iTimebase = iTimebase;
	pthread_mutex_unlock(&mutex);
}

int timecode::get_timebase()
{
	pthread_mutex_lock(&mutex);
	int timebase = m_iTimebase;
	pthread_mutex_unlock(&mutex);
	return timebase;
}

void timecode::set_framecount(int iFramesCount)
{
	pthread_mutex_lock(&mutex);
	m_iFrameCount = iFramesCount;
	pthread_mutex_unlock(&mutex);
}

int timecode::get_framecount()
{
	pthread_mutex_lock(&mutex);
	int framecount = m_iFrameCount;
	pthread_mutex_unlock(&mutex);
	return framecount;
}

void timecode::set_timecode(std::string strTimecode)
{
	// Parse timecode
	if (strTimecode.length() != 11)
		return;

	const bool bDropFrame = strTimecode.substr(8, 1) == ";";
	int iHours, iMinutes, iSeconds, iFrames;

	try
	{
		iHours = boost::lexical_cast<int>(strTimecode.substr(0, 2));
		iMinutes = boost::lexical_cast<int>(strTimecode.substr(3, 2));
		iSeconds = boost::lexical_cast<int>(strTimecode.substr(6, 2));
		iFrames = boost::lexical_cast<int>(strTimecode.substr(9, 2));
	}
	catch (boost::bad_lexical_cast &)
	{
		return;
	}

	// Convert timecode to frame count
	pthread_mutex_lock(&mutex);

	const int iFramesPerSecond = m_iTimebase;
	const int iFramesPerMinute = m_iTimebase * 60;
	const int iFramesPerHour = m_iTimebase * 60 * 60;

	m_iFrameCount = 
		iFramesPerHour * iHours
		+ iFramesPerMinute * iMinutes
		+ iFramesPerSecond * iSeconds
		+ iFrames;

	if (bDropFrame)
	{
		const int iDropFrames = m_iTimebase * round(6.0 / 100.0);
		const int iTotalMinutes = 60 * iHours + iMinutes;
		m_iFrameCount -= iDropFrames * (iTotalMinutes - iTotalMinutes / 10);
	}

	pthread_mutex_unlock(&mutex);
}

std::string timecode::get_timecode(bool bDropFrame)
{
	pthread_mutex_lock(&mutex);
	int iFrameCount = m_iFrameCount;

	// Convert frame count to timecode
	if (bDropFrame)
	{
		const int iDropFrames = m_iTimebase * round(6.0 / 100.0);
		const int iFramesPer10Minutes = m_iTimebase * 60 * 10;
		const int iFramesPerMinute = m_iTimebase * 60 - iDropFrames;
		
		const int iD = iFrameCount / iFramesPer10Minutes;
		const int iM = iFrameCount % iFramesPer10Minutes;
		
		iFrameCount += iDropFrames * 9 * iD;
		
		if (iM > 1)
			iFrameCount += iDropFrames * ((iM - iDropFrames) / iFramesPerMinute);
	}

	const int iFrames = iFrameCount % m_iTimebase;
	const int iSeconds = (iFrameCount / m_iTimebase) % 60;
	const int iMinutes = ((iFrameCount / m_iTimebase) / 60) % 60;
	const int iHours = (((iFrameCount / m_iTimebase) / 60) / 60) % 24;

	// Format the timecode
	std::string strTimecode;
	if (bDropFrame)
		strTimecode = str( boost::format("%02d:%02d:%02d;%02d") % iHours % iMinutes % iSeconds % iFrames );
	else
		strTimecode = str( boost::format("%02d:%02d:%02d:%02d") % iHours % iMinutes % iSeconds % iFrames );

	pthread_mutex_unlock(&mutex);
	return strTimecode;
}

} // namespace model
} // namespace iola
