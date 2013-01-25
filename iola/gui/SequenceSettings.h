/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
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

// BOOST
#include <boost/rational.hpp>

// FLTK
#include "fltk.h"

// IOLA
#include <iola/model/ivideo_settings.h>

namespace iola
{
namespace gui
{

/////////////////////////////////////////////////////////////////////////////
// class iola::gui::SequenceSettings

/// Sequence settings dialog
class SequenceSettings :
	public Fl_Double_Window,
	public iola::model::ivideo_settings
{
public:
	SequenceSettings();
	~SequenceSettings();

	void connect_to(iola::model::ivideo_settings* settings);

	void update_settings_on(iola::model::ivideo_settings* object);
	void update_settings_on(iola::model::ivideo_characteristics* object);
	void set_width(const int width);
	void set_height(const int height);
	void set_par(const iola::model::ivideo_settings::par_t par);
	void set_anamorphic(const bool anamorphic);
	void set_field_dominance(const iola::model::ivideo_settings::field_t field);
	void set_fps_timebase(const int timebase);
	void set_fps_ntsc(const bool ntsc);

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

	iola::model::ivideo_settings* m_pkSettings;

	bool m_bEdited;

	int m_iTimebase;
	bool m_bNTSC;

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
