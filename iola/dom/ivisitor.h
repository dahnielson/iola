/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_DOM_IVISITOR_H
#define IOLA_DOM_IVISITOR_H

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

#include <iola/iunknown.h>

namespace iola
{
namespace dom
{

class anamorphic_element;
class depth_element;
class duration_element;
class end_element;
class fielddominance_element;
class height_element;
class in_element;
class name_element;
class ntsc_element;
class out_element;
class pathurl_element;
class pixelaspectratio_element;
class samplerate_element;
class start_element;
class string_element;
class timebase_element;
class uuid_element;
class width_element;

////////////////////////////////////////////////////////////////////////////
// class iola::dom::ivisitor

class ivisitor :
	virtual public iola::iunknown
{
public:
	virtual void visit(iola::dom::anamorphic_element* element) = 0;
	virtual void visit(iola::dom::depth_element* element) = 0;
	virtual void visit(iola::dom::duration_element* element) = 0;
	virtual void visit(iola::dom::end_element* element) = 0;
	virtual void visit(iola::dom::fielddominance_element* element) = 0;
	virtual void visit(iola::dom::height_element* element) = 0;
	virtual void visit(iola::dom::in_element* element) = 0;
	virtual void visit(iola::dom::name_element* element) = 0;
	virtual void visit(iola::dom::ntsc_element* element) = 0;
	virtual void visit(iola::dom::out_element* element) = 0;
	virtual void visit(iola::dom::pathurl_element* element) = 0;
	virtual void visit(iola::dom::pixelaspectratio_element* element) = 0;
	virtual void visit(iola::dom::samplerate_element* element) = 0;
	virtual void visit(iola::dom::start_element* element) = 0;
	virtual void visit(iola::dom::string_element* element) = 0;
	virtual void visit(iola::dom::timebase_element* element) = 0;
	virtual void visit(iola::dom::uuid_element* element) = 0;
	virtual void visit(iola::dom::width_element* element) = 0;
};

} // namespace dom
} // namespace iola

#endif // IOLA_DOM_IVISITOR_H
