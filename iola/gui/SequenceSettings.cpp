/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */

// Iola NLE
// Copyright (c) 2010-2013, Anders Dahnielson
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

// RLOG
#include <rlog/rlog.h>

// IOLA
#include "SequenceSettings.h"

namespace iola
{
namespace gui
{

SequenceSettings::SequenceSettings() :
	Fl_Double_Window(350, 165, "Sequence settings"),
	m_pkSettings(0),
	m_bEdited(false)
{
	Fl_Pack* pkMainGroup = new Fl_Pack(10, 10, 330, 145);
	pkMainGroup->spacing(5);

	// Frame Size
	Fl_Pack* pkFrameSizeGroup = new Fl_Pack(0, 0, 330, 20);
	pkFrameSizeGroup->type(Fl_Pack::HORIZONTAL);
	pkFrameSizeGroup->spacing(5);
	Fl_Box* pkFrameSizeLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 100, 20, "Frame Size:");
	pkFrameSizeLabel->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	pkFrameSizeLabel->labelsize(11);
	m_pkWidth = new Fl_Value_Input(0, 0, 40, 20);
	m_pkWidth->step(1);
	m_pkWidth->bounds(0, 1000000);
	m_pkWidth->textsize(11);
	Fl_Box* pkFrameSizeDiv = new Fl_Box(FL_FLAT_BOX, 0, 0, 10, 20, "x");
	pkFrameSizeDiv->labelsize(11);
	m_pkHeight = new Fl_Value_Input(0, 0, 40, 20);
	m_pkHeight->step(1);
	m_pkHeight->bounds(0, 1000000);
	m_pkHeight->textsize(11);
	m_pkDAR = new Fl_Choice(0, 0, 120, 20);
	m_pkDAR->textsize(11);
	m_pkDAR->callback((Fl_Callback *)dar_callback, this);
	pkFrameSizeGroup->end();

	// Pixel Aspect Ratio
	Fl_Pack* pkPARGroup = new Fl_Pack(0, 0, 330, 20);
	pkPARGroup->type(Fl_Pack::HORIZONTAL);
	pkPARGroup->spacing(5);
	Fl_Box* pkPARLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 100, 20, "Pixel Aspect Ratio:");
	pkPARLabel->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	pkPARLabel->labelsize(11);
	m_pkPAR = new Fl_Choice(0, 0, 120, 20);
	m_pkPAR->textsize(11);
	m_pkAnamorphic = new Fl_Check_Button(0, 0, 100, 20, " Anamorphic 16:9");
	m_pkAnamorphic->labelsize(11);
	pkPARGroup->end();

	// Field Dominance
	Fl_Pack* pkFieldDominanceGroup = new Fl_Pack(0, 0, 330, 20);
	pkFieldDominanceGroup->type(Fl_Pack::HORIZONTAL);
	pkFieldDominanceGroup->spacing(5);
	Fl_Box* pkFieldDominanceLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 100, 20, "Field Dominance:");
	pkFieldDominanceLabel->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	pkFieldDominanceLabel->labelsize(11);
	m_pkFieldDominance = new Fl_Choice(0, 0, 100, 20);
	m_pkFieldDominance->textsize(11);
	pkFieldDominanceGroup->end();

	// Frame Rate
	Fl_Pack* pkFPSGroup = new Fl_Pack(0, 0, 330, 20);
	pkFPSGroup->type(Fl_Pack::HORIZONTAL);
	pkFPSGroup->spacing(5);
	Fl_Box* pkFPSLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 100, 20, "Frame Rate:");
	pkFPSLabel->align(FL_ALIGN_RIGHT|FL_ALIGN_INSIDE);
	pkFPSLabel->labelsize(11);
	m_pkFPS = new Fl_Choice(0, 0, 60, 20);
	m_pkFPS->textsize(11);
	pkFPSGroup->end();

	// Filler
	Fl_Box* pkMainFiller = new Fl_Box(0, 0, 330, 20);

	// OK and Cancel
	Fl_Pack* pkButtonGroup = new Fl_Pack(0, 0, 320, 20);
	pkButtonGroup->type(Fl_Pack::HORIZONTAL);
	pkButtonGroup->spacing(5);
	Fl_Box* pkButtonFiller = new Fl_Box(0, 0, 170, 20);
	Fl_Button* pkOkButton = new Fl_Button(0, 0, 75, 20, "OK");
	pkOkButton->labelsize(11);
	pkOkButton->type(FL_NORMAL_BUTTON);
	pkOkButton->callback((Fl_Callback *)ok_callback, this);
	Fl_Button* pkCancelButton = new Fl_Button(0, 0, 75, 20, "Cancel");
	pkCancelButton->labelsize(11);
	pkCancelButton->type(FL_NORMAL_BUTTON);
	pkCancelButton->shortcut(FL_Escape);
	pkCancelButton->callback((Fl_Callback *)cancel_callback, this);
	pkButtonGroup->end();

	pkMainGroup->end();

	// Window
	xclass("iola");
	callback((Fl_Callback *)close_window, this);
	set_modal();
	end();

	// Fill in
	m_pkDAR->add("NTSC (ITU-R 601)", 0, 0);
	m_pkDAR->add("PAL (ITU-R 601)", 0, 0);
	m_pkDAR->add("HD (960x720)", 0, 0);
	m_pkDAR->add("HD (1280x720)", 0, 0);
	m_pkDAR->add("HD (1280x1080)", 0, 0);
	m_pkDAR->add("HD (1440x1080)", 0, 0);
	m_pkDAR->add("HD (1920x1080)", 0, 0);
	m_pkDAR->add("Custom", 0, 0);

	m_pkPAR->add("Square", 0, 0);
	m_pkPAR->add("NTSC (ITU-R 601)", 0, 0);
	m_pkPAR->add("PAL (ITU-R 601)", 0, 0);
	m_pkPAR->add("HD (920x720)", 0, 0);
	m_pkPAR->add("HD (1280x1080)", 0, 0);
	m_pkPAR->add("HD (1440x1080)", 0, 0);

	m_pkFieldDominance->add("None", 0, 0);
	m_pkFieldDominance->add("Lower (Even)", 0, 0);
	m_pkFieldDominance->add("Upper (Odd)", 0, 0);

	m_pkFPS->add("23.98", 0, 0);
	m_pkFPS->add("24", 0, 0);
	m_pkFPS->add("25", 0, 0);
	m_pkFPS->add("29.97", 0, 0);
	m_pkFPS->add("30", 0, 0);
	m_pkFPS->add("50", 0, 0);
	m_pkFPS->add("59.95", 0, 0);
	m_pkFPS->add("60", 0, 0);
}

SequenceSettings::~SequenceSettings()
{
}

void SequenceSettings::connect_to(iola::model::ivideo_settings* settings)
{
	m_pkSettings = settings;
}

void SequenceSettings::update_settings_on(iola::model::ivideo_settings* object)
{
	// Frame Size
	object->set_width(m_pkWidth->value());
	object->set_height(m_pkHeight->value());

	// Pixel Aspect Ratio
	switch (m_pkPAR->value())
	{
	case 0: // Square
		object->set_par(iola::model::ivideo_settings::SQUARE);
		break;
	case 1: // NTSC (ITU-R 601)
		object->set_par(iola::model::ivideo_settings::NTSC_601);
		break;
	case 2: // PAL (ITU-R 601)
		object->set_par(iola::model::ivideo_settings::PAL_601);
		break;
	case 3: // HD (920x720)
		object->set_par(iola::model::ivideo_settings::HD_960x720);
		break;
	case 4: // HD (1280x1080)
		object->set_par(iola::model::ivideo_settings::HD_1280x1080);
		break;
	case 5: // HD (1440x1080)
		object->set_par(iola::model::ivideo_settings::HD_1440x1080);
		break;
	default:
		rError("%s: Pixel Aspect Ratio choice #%i '%s' has not been implemented", 
		       __PRETTY_FUNCTION__, m_pkPAR->value(), m_pkPAR->text());
	};

	object->set_anamorphic(m_pkAnamorphic->value());

	// Field Dominance
	switch (m_pkFieldDominance->value())
	{
	case 0: // None
		object->set_field_dominance(iola::model::ivideo_settings::NONE);
		break;
	case 1: // Lower (Even)
		object->set_field_dominance(iola::model::ivideo_settings::EVEN);
		break;
	case 2: // Upper (Odd)
		object->set_field_dominance(iola::model::ivideo_settings::ODD);
		break;
	default:
		rError("%s: Field Dominance choice #%i '%s' has not been implemented", 
		       __PRETTY_FUNCTION__, m_pkFieldDominance->value(), m_pkFieldDominance->text());
	};

	// Timebase
	switch (m_pkFPS->value())
	{
	case 0: // 23.98
		object->set_fps_timebase(24);
		object->set_fps_ntsc(true);
		break;
	case 1: // 24
		object->set_fps_timebase(24);
		object->set_fps_ntsc(false);
		break;
	case 2:// 25
		object->set_fps_timebase(25);
		object->set_fps_ntsc(false);
		break;
	case 3: // 29.97
		object->set_fps_timebase(30);
		object->set_fps_ntsc(true);
		break;
	case 4: // 30
		object->set_fps_timebase(30);
		object->set_fps_ntsc(false);
		break;
	case 5: // 50
		object->set_fps_timebase(50);
		object->set_fps_ntsc(false);
		break;
	case 6: // 59.95
		object->set_fps_timebase(60);
		object->set_fps_ntsc(true);
		break;
	case 7: // 60
		object->set_fps_timebase(60);
		object->set_fps_ntsc(false);
		break;
	default:
		rError("%s: Timebase choice #%i '%s' has not been implemented", 
		       __PRETTY_FUNCTION__, m_pkFPS->value(), m_pkFPS->text());
	};
}

void  SequenceSettings::update_settings_on(iola::model::ivideo_characteristics* object)
{
	assert(!"Unimplemented");
}

void SequenceSettings::set_width(const int width)
{
	m_pkWidth->value(width);
}

void SequenceSettings::set_height(const int height)
{
	m_pkHeight->value(height);
}

void SequenceSettings::set_par(const iola::model::ivideo_settings::par_t par)
{
	switch (par)
	{
	case iola::model::ivideo_settings::SQUARE:
		m_pkPAR->value(0);
		break;
	case iola::model::ivideo_settings::NTSC_601:
		m_pkPAR->value(1);
		break;
	case iola::model::ivideo_settings::PAL_601:
		m_pkPAR->value(2);
		break;
	case iola::model::ivideo_settings::HD_960x720:
		m_pkPAR->value(3);
		break;
	case iola::model::ivideo_settings::HD_1280x1080:
		m_pkPAR->value(4);
		break;
	case iola::model::ivideo_settings::HD_1440x1080:
		m_pkPAR->value(5);
		break;
	default:
		rError("%s: Unknown Pixel Aspect Ratio", __PRETTY_FUNCTION__);
	};
}

void SequenceSettings::set_anamorphic(const bool anamorphic)
{
	m_pkAnamorphic->value(anamorphic);
}

void SequenceSettings::set_field_dominance(const iola::model::ivideo_settings::field_t field)
{
	switch (field)
	{
	case iola::model::ivideo_settings::NONE:
		m_pkFieldDominance->value(0);
		break;
	case iola::model::ivideo_settings::EVEN:
		m_pkFieldDominance->value(1);
		break;
	case iola::model::ivideo_settings::ODD:
		m_pkFieldDominance->value(2);
		break;
	default:
		rError("%s: Unknown Field Dominance", __PRETTY_FUNCTION__);
	};
}

void SequenceSettings::set_fps_timebase(const int timebase)
{
	m_iTimebase = timebase;
}

void SequenceSettings::set_fps_ntsc(const bool ntsc)
{
	m_bNTSC = ntsc;
}

int SequenceSettings::handle(int event)
{
	switch (event)
	{
	case FL_SHOW:
		rDebug("%s: Show sequence settings dialog", __PRETTY_FUNCTION__);
		if (!m_bEdited)
			get_sequence_settings();
		m_bEdited = true;
		return Fl_Double_Window::handle(event);
	default:
		return Fl_Double_Window::handle(event);
	};
}

void SequenceSettings::get_sequence_settings()
{
	if (!m_pkSettings)
		return;

	m_pkSettings->update_settings_on(this);

	// Frame Size
	if (m_pkWidth->value() == 720 && m_pkHeight->value() == 480)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(0);
	}
	else if (m_pkWidth->value() == 720 && m_pkHeight->value() == 576)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(1);
	}
	else if (m_pkWidth->value() == 960 && m_pkHeight->value() == 720)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(2);
	}
	else if (m_pkWidth->value() == 1280 && m_pkHeight->value() == 720)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(3);
	}
	else if (m_pkWidth->value() == 1280 && m_pkHeight->value() == 1080)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(4);
	}
	else if (m_pkWidth->value() == 1440 && m_pkHeight->value() == 1080)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(5);
	}
	else if (m_pkWidth->value() == 1920 && m_pkHeight->value() == 1080)
	{
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkDAR->value(6);
	}
	else
	{
		m_pkWidth->activate();
		m_pkHeight->activate();
		m_pkDAR->value(7);
	}

	// Timebase
	switch (m_iTimebase)
	{
	case 24:
		if (m_bNTSC)
			m_pkFPS->value(0); // 23.98
		else
			m_pkFPS->value(1); // 24
		break;
	case 25:
		if (m_bNTSC)
			rError("%s: Invalid Timebase and NTSC flag combination", __PRETTY_FUNCTION__);
		else
			m_pkFPS->value(2); // 25
		break;
	case 30:
		if (m_bNTSC)
			m_pkFPS->value(3); // 29.97
		else
			m_pkFPS->value(4); // 30
		break;
	case 50:
		if (m_bNTSC)
			rError("%s: Invalid Timebase and NTSC flag combination", __PRETTY_FUNCTION__);
		else
			m_pkFPS->value(5); // 50
		break;
	case 60:
		if (m_bNTSC)
			m_pkFPS->value(6); // 59.95
		else
			m_pkFPS->value(7); // 60
		break;
	default:
		rError("%s: Unknown Timebase", __PRETTY_FUNCTION__);
	};
}

void SequenceSettings::set_sequence_settings()
{
	if (!m_pkSettings)
		return;

	update_settings_on(m_pkSettings);
}

void SequenceSettings::dar_callback()
{
	switch (m_pkDAR->value())
	{
	case 0: // NTSC (ITU-R 601)
		m_pkAnamorphic->activate();
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(720);
		m_pkHeight->value(480);
		break;
	case 1: // PAL (ITU-R 601)
		m_pkAnamorphic->activate();
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(720);
		m_pkHeight->value(576);
		break;
	case 2: // HD (960x720)
		m_pkAnamorphic->deactivate();
		m_pkAnamorphic->value(0);
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(960);
		m_pkHeight->value(720);
		break;
	case 3: // HD (1280x720)
		m_pkAnamorphic->deactivate();
		m_pkAnamorphic->value(0);
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(1280);
		m_pkHeight->value(720);
		break;
	case 4: // HD (1280x1080)
		m_pkAnamorphic->deactivate();
		m_pkAnamorphic->value(0);
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(1280);
		m_pkHeight->value(1080);
		break;
	case 5: // HD (1440x1080)
		m_pkAnamorphic->deactivate();
		m_pkAnamorphic->value(0);
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(1440);
		m_pkHeight->value(1080);
		break;
	case 6: // HD (1920x1080)
		m_pkAnamorphic->deactivate();
		m_pkAnamorphic->value(0);
		m_pkWidth->deactivate();
		m_pkHeight->deactivate();
		m_pkWidth->value(1920);
		m_pkHeight->value(1080);
		break;
	case 7: // Custom
		m_pkAnamorphic->activate();
		m_pkWidth->activate();
		m_pkHeight->activate();
		break;
	default:
		rError("%s: Display Aspect Ratio choice #%i '%s' has not been implemented", 
		       __PRETTY_FUNCTION__, m_pkDAR->value(), m_pkDAR->text());
		return;
	};
}

void SequenceSettings::ok_callback()
{
	hide();
	m_bEdited = false;
	set_sequence_settings();
}

void SequenceSettings::cancel_callback()
{
	hide();
	m_bEdited = false;
}

void SequenceSettings::close_window()
{
	if (Fl::event() == FL_SHORTCUT && Fl::event_key() == FL_Escape) 
		return;

	hide();
	m_bEdited = false;
}

} // namespace gui
} // namespace iola
