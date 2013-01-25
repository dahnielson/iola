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

// BOOST
#include <boost/rational.hpp>

// IOLA
#include "ivideo_settings.h"
#include "ivideo_characteristics.h"

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::video_settings_implementation

class video_settings_implementation :
		public iola::model::ivideo_settings
{
public:
	video_settings_implementation() :
		m_iWidth(720),
		m_iHeight(480),
		m_iPAR(iola::model::ivideo_settings::NTSC_601),
		m_bAnamorphic(false),
		m_iFieldDominance(iola::model::ivideo_settings::EVEN),
		m_iTimebase(30),
		m_bNTSC(true)
	{
	}

	~video_settings_implementation()
	{
	}

	void update_settings_on(iola::model::ivideo_settings* object)
	{
		object->set_width(m_iWidth);
		object->set_height(m_iHeight);
		object->set_par(m_iPAR);
		object->set_anamorphic(m_bAnamorphic);
		object->set_field_dominance(m_iFieldDominance);
		object->set_fps_timebase(m_iTimebase);
		object->set_fps_ntsc(m_bNTSC);
	}

	void update_settings_on(iola::model::ivideo_characteristics* object)
	{
		//NOTE This is the DAR of the PA not the CA
		boost::rational<int> SAR(m_iWidth, m_iHeight);
		boost::rational<int> DAR = SAR / boost::gcd(m_iWidth, m_iHeight);

		boost::rational<int> PAR;
		switch (m_iPAR)
		{
		case SQUARE:
			PAR = boost::rational<int>(1, 1);
			break;
		case NTSC_601:
			PAR = boost::rational<int>(10, 11);
			if (m_bAnamorphic)
				PAR *= 4;
			break;
		case PAL_601:
			PAR = boost::rational<int>(59, 54);
			if (m_bAnamorphic)
				PAR *= 4;
			break;
		case HD_1280x1080:
			PAR = boost::rational<int>(3, 2);
			break;
		case HD_960x720:
		case HD_1440x1080:
			PAR = boost::rational<int>(4, 3);
			break;
		};

		boost::rational<int> FPS;
		if (m_bNTSC)
			FPS = boost::rational<int>(m_iTimebase * 1000, 1001);
		else
			FPS = boost::rational<int>(m_iTimebase, 1);

		bool progressive;
		switch (m_iFieldDominance)
		{
		case NONE:
			progressive = true;
		case EVEN:
		case ODD:
			progressive = false;
		};

		object->set_width(m_iWidth);
		object->set_height(m_iHeight);
		object->set_dar(DAR);
		object->set_par(PAR);
		object->set_fps(FPS);
		object->set_progressive(progressive);
	}

	void set_width(const int width)
	{
		m_iWidth = width;
		on_width_change_signal(m_iWidth);
	}

	void set_height(const int height)
	{
		m_iHeight = height;
		on_height_change_signal(m_iHeight);
	}

	void set_par(const par_t par)
	{
		m_iPAR = par;
		//on_par_change_signal(); //FIXME need rational
	}

	void set_anamorphic(const bool anamorphic)
	{
		m_bAnamorphic = anamorphic;
		//on_par_change_signal() //FIXME need rational
	}

	void set_field_dominance(const field_t field)
	{
		m_iFieldDominance = field;
		//on_progressive_change_signal //FIXME
	}

	void set_fps_timebase(const int timebase)
	{
		m_iTimebase = timebase;
		on_timebase_change_signal(m_iTimebase);
	}

	void set_fps_ntsc(const bool ntsc)
	{
		m_bNTSC = ntsc;
		on_ntsc_change_signal(m_bNTSC);
	}

private:
	int m_iWidth;
	int m_iHeight;
	par_t m_iPAR;
	bool m_bAnamorphic;
	field_t m_iFieldDominance;
	int m_iTimebase;
	bool m_bNTSC;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_video_settings

ivideo_settings* create_video_settings()
{
	return new video_settings_implementation();
}

} // namespace model
} // namespace iola
