/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_APPLICATION_IAPPLICATION_H
#define IOLA_APPLICATION_IAPPLICATION_H

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
#include <iola/model/iproject.h>

namespace iola
{
namespace application
{

/////////////////////////////////////////////////////////////////////////////
// class iola::application::iapplication

/// Abstract interface for the application
class iapplication :
	public virtual iola::iunknown
{
public:
	/// Return the project object
	virtual iola::model::iproject* get_project() = 0;

	/// Enter main loop
	virtual void run() = 0;
	/// Exit application
	virtual void quit() = 0;

protected:
	iapplication()
	{}
	iapplication(const iapplication& RHS);
	iapplication& operator=(const iapplication& RHS)
	{
		return * this;
	}
	virtual ~iapplication()
	{}
};

} // namespace application
} // namespace iola

#endif // IOLA_APPLICATION_IAPPLICATION_H
