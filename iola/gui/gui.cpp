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

// FLTK
#include <FL/Fl.H>

// IOLA
#include "igui.h"
#include "MainWindow.h"
#include "theme.h"

namespace
{

class gui_implementation :
	public iola::gui::igui
{
public:
	gui_implementation() :
		m_bRun(false),
		m_pkMainWindow(new iola::gui::MainWindow())
	{
	}

	~gui_implementation()
	{
		delete m_pkMainWindow;
	}

	bool show()
	{
		Fl::visible_focus(0);
		Fl::visual(FL_DOUBLE|FL_RGB);
		iola::gui::scheme::theme();

		m_pkMainWindow->show();

		m_bRun = true;
		while(m_bRun)
			Fl::wait(0); //NOTE The zero time makes the GUI really responsive

		return true;
	}

	void stop()
	{
		m_bRun = false;
	}

private:
	bool m_bRun;
	iola::gui::MainWindow* m_pkMainWindow;
};

} // namespace

namespace iola
{
namespace gui
{

/////////////////////////////////////////////////////////////////////////////
// function iola::gui::factory

iola::gui::igui* factory()
{
	return new gui_implementation();
}

} // namespace gui
} // namespace iola
