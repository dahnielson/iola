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

// IOLA
#include "iaudio_settings.h"
#include "iaudio_characteristics.h"

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::audio_settings_implementation

class audio_settings_implementation :
	public iola::model::iaudio_settings
{
public:
	audio_settings_implementation() :
		m_iDepth(16),
		m_iRate(48000)
	{
	}

	~audio_settings_implementation()
	{
	}

	void update_settings_on(iola::model::iaudio_settings* object)
	{
		object->set_sample_depth(m_iDepth);
		object->set_sample_rate(m_iRate);
	}

	void update_settings_on(iola::model::iaudio_characteristics* object)
	{
		object->set_sample_depth(m_iDepth);
		object->set_sample_rate(m_iRate);
	}

	void set_sample_depth(const int depth)
	{
		m_iDepth = depth;
		on_sample_depth_change_signal(m_iDepth);
	}

	void set_sample_rate(const int rate)
	{
		m_iRate = rate;
		on_sample_rate_change_signal(m_iRate);
	}

private:
	int m_iDepth;
	int m_iRate;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_audio_settings

iaudio_settings* create_audio_settings()
{
	return new audio_settings_implementation();
}

} // namespace model
} // namespace iola
