/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_MODEL_IVIDEO_CHARACTERISTICS_H
#define IOLA_MODEL_IVIDEO_CHARACTERISTICS_H

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
#include <iola/iunknown.h>

namespace iola
{
namespace model
{

class iplayable;

////////////////////////////////////////////////////////////////////////////
// class iola::model::ivideo_characteristics

/// Abstract interface
class ivideo_characteristics :
	virtual public iola::iunknown
{
public:
	/// Set storage width of frame in pixels
	virtual void set_width(const int width) = 0;
	/// Set storage width of frame in pixels
	virtual void set_height(const int height) = 0;
	/// Set Display Aspect Ratio
	virtual void set_dar(const boost::rational<int> dar) = 0;
	/// Set Pixel Aspect Ratio
	virtual void set_par(const boost::rational<int> par) = 0;
	/// Set frame rate
	virtual void set_fps(const boost::rational<int> fps) = 0;
	/// Set if scan is progressive
	virtual void set_progressive(const bool progressive) = 0;
};

} // namespace model
} // namespace iola

#endif // IOLA_MODEL_IVIDEO_CHARACTERISTICS_H
