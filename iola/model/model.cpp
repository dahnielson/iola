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
#include <cassert>
#include <exception>

// RLOG
#include <rlog/rlog.h>

// IOLA
#include <iola/model/imodel.h>
#include <iola/model/iprogram.h>
#include <iola/model/isource.h>

#include <iola/model/program.h>
#include <iola/model/source.h>

namespace
{

////////////////////////////////////////////////////////////////////////////
// class <unnamed>::model_implementation

class model_implementation :
	public iola::model::imodel
{
public:
	model_implementation() :
		m_pkSource(iola::model::create_source()),
		m_pkProgram(iola::model::create_program())
	{
		assert(m_pkSource);
		assert(m_pkProgram);
		m_pkSource->on_alert_signal.connect(on_alert_signal);
		m_pkProgram->on_alert_signal.connect(on_alert_signal);
	}

	~model_implementation()
	{
		assert(m_pkSource);
		assert(m_pkProgram);
		delete m_pkSource;
		delete m_pkProgram;
	}

	iola::model::isource* source()
	{
		assert(m_pkSource);
		return m_pkSource;
	}

	iola::model::iprogram* program()
	{
		assert(m_pkProgram);
		return m_pkProgram;
	}

	void make_lift_edit()
	{
		assert(m_pkProgram);
		m_pkProgram->make_lift_edit();
	}

	void make_extract_edit()
	{
		assert(m_pkProgram);
		m_pkProgram->make_extract_edit();
	}

	void make_insert_edit()
	{
		assert(m_pkSource);
		assert(m_pkProgram);
		try
		{
			m_pkProgram->make_insert_edit(m_pkSource->clip());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
		}
	}

	void make_overwrite_edit()
	{
		assert(m_pkSource);
		assert(m_pkProgram);
		try
		{
			m_pkProgram->make_overwrite_edit(m_pkSource->clip());
		}
		catch (std::exception& e)
		{
			rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
		}
	}

private:
	iola::model::isource* m_pkSource;
	iola::model::iprogram* m_pkProgram;
};

} // namespace

namespace iola
{
namespace model
{

////////////////////////////////////////////////////////////////////////////
// function iola::model::create_model

imodel* create_model()
{
	return new model_implementation();
}

} // namespace model
} // namespace iola
