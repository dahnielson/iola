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

// IOLA
#include <iola/model/imarkers.h>
#include <iola/model/imarker_characteristics.h>
#include <iola/model/iplayhead.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::markers_implementation

class markers_implementation :
	public iola::model::imarkers
{
public:
	markers_implementation() :
		m_iIn(-1),
		m_iOut(-1),
		m_iStart(-1),
		m_iEnd(-1)
	{
	}

	~markers_implementation()
	{
	}

	void update_markers_on(iola::model::imarker_characteristics* object)
	{
		object->set_markers(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	void update_markers_on(iola::model::imarkers* markers)
	{
		markers->set_in_point(m_iIn);
		markers->set_out_point(m_iOut);
		markers->set_start_point(m_iStart);
		markers->set_end_point(m_iEnd);
	}

	void set_in_point_on(iola::model::imarkers* markers)
	{
		markers->set_in_point(m_iIn);
	}

	void set_out_point_on(iola::model::imarkers* markers)
	{
		markers->set_out_point(m_iOut);
	}

	void set_start_point_on(iola::model::imarkers* markers)
	{
		markers->set_start_point(m_iStart);
	}

	void set_end_point_on(iola::model::imarkers* markers)
	{
		markers->set_end_point(m_iEnd);
	}

	void set_in_point(const int frame)
	{
		if (m_iOut != -1 && frame != -1 && frame > m_iOut)
			throw std::range_error("In point larger than out point");

		m_iIn = frame;
		on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	void set_out_point(const int frame)
	{
		if (m_iIn != -1 && frame != -1 && frame < m_iIn)
			throw std::range_error("Out point lesser than in point");

		m_iOut = frame;
		on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	void set_start_point(const int frame)
	{
		if (m_iEnd != -1 && frame != -1 && frame > m_iEnd)
			throw std::range_error("Start point larger than end point");

		m_iStart = frame;
		on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	void set_end_point(const int frame)
	{
		if (m_iStart != -1 && frame != -1 && frame < m_iStart)
			throw std::range_error("End point lesser than start point");

		m_iEnd = frame;
		on_mark_change_signal(m_iIn, m_iOut, m_iStart, m_iEnd);
	}

	void goto_in_point_on(iola::model::iplayhead* object)
	{
		object->set_frame(m_iIn);
	}

	void goto_out_point_on(iola::model::iplayhead* object)
	{
		object->set_frame(m_iOut);
	}

	void goto_start_point_on(iola::model::iplayhead* object)
	{
		object->set_frame(m_iStart);
	}

	void goto_end_point_on(iola::model::iplayhead* object)
	{
		object->set_frame(m_iEnd);
	}

private:
	int m_iIn;
	int m_iOut;
	int m_iStart;
	int m_iEnd;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_markers

imarkers* create_markers()
{
	return new markers_implementation();
}

} // namespace model
} // namespace iola
