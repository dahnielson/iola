/* -*- Mode: C++ ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_SEQUENCESETTINGS_H
#define IOLA_GUI_SEQUENCESETTINGS_H

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
#include "fltk.h"

namespace iola
{
namespace gui
{

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::SequenceSettings

/// Sequence settings dialog
class SequenceSettings :
	public Fl_Double_Window
{
public:
	SequenceSettings();
	~SequenceSettings();

	/// FLTK event handler
	int handle(int event);

private:
	void set_sequence_settings();
	void get_sequence_settings();

	static void dar_callback(Fl_Widget*, void* v) { reinterpret_cast<SequenceSettings*>(v)->dar_callback();	}
	static void ok_callback(Fl_Widget*, void* v) { reinterpret_cast<SequenceSettings*>(v)->ok_callback(); }
	static void cancel_callback(Fl_Widget*, void* v) { reinterpret_cast<SequenceSettings*>(v)->cancel_callback(); }
	static void close_window(Fl_Widget*, void* v) { reinterpret_cast<SequenceSettings*>(v)->close_window(); }

	void dar_callback();
	void ok_callback();
	void cancel_callback();
	void close_window();

	bool m_bEdited;

	Fl_Value_Input* m_pkWidth;
	Fl_Value_Input* m_pkHeight;
	Fl_Choice* m_pkDAR;
	Fl_Choice* m_pkPAR;
	Fl_Check_Button* m_pkAnamorphic;
	Fl_Choice* m_pkFieldDominance;
	Fl_Choice* m_pkFPS;
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_SEQUENCESETTINGS_H
