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
#include "About.h"

namespace iola
{
namespace gui
{

About::About() :
	Fl_Double_Window(350, 210, "About Iola NLE")
{
	Fl_Pack* pkMainGroup = new Fl_Pack(10, 10, 330, 190);
	pkMainGroup->spacing(5);

	Fl_Pack* pkLabelGroup = new Fl_Pack(0, 0, 330, 40);
	pkLabelGroup->spacing(0);
	Fl_Box* pkProgramNameLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 330, 20, "Iola NLE");
	pkProgramNameLabel->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	pkProgramNameLabel->labelsize(12);
	pkProgramNameLabel->labelfont(FL_HELVETICA_BOLD);
	Fl_Box* pkProgramVersionLabel = new Fl_Box(FL_FLAT_BOX, 0, 0, 330, 20, "version 0.0");
	pkProgramVersionLabel->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	pkProgramVersionLabel->labelsize(11);
	pkLabelGroup->end();

	Fl_Tabs* pkTabsGroup = new Fl_Tabs(0, 0, 330, 120);
	Fl_Group *pkAboutGroup = new Fl_Group(0, 20, 330, 100, "About");
	pkAboutGroup->labelsize(11);
	Fl_Box* pkAboutLabel = new Fl_Box(FL_FLAT_BOX, 10, 30, 310, 70,
					    "A free non-linear digital video and motion picture editor.\n\nCopyright (c) 2010, Anders Dahnielson.");
	pkAboutLabel->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
	pkAboutLabel->labelsize(11);
	pkAboutGroup->end();
	Fl_Group *pkAuthorsGroup = new Fl_Group(0, 20, 330, 100, "Authors");
	pkAuthorsGroup->labelsize(11);
	Fl_Box* pkAuthorsLabel = new Fl_Box(FL_FLAT_BOX, 10, 30, 310, 70, 
					    "Anders Dahnielson\n  <anders@@danielson.com>\n  Developer");
	pkAuthorsLabel->align(FL_ALIGN_TOP|FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
	pkAuthorsLabel->labelsize(11);
	pkAuthorsGroup->end();
	Fl_Group *pkLicenseGroup = new Fl_Group(0, 20, 330, 100, "License");
	pkLicenseGroup->labelsize(11);
	Fl_Box* pkLicenseLabel = new Fl_Box(FL_FLAT_BOX, 10, 30, 310, 70, 
					    "GNU General Public License Version 2");
	pkLicenseLabel->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
	pkLicenseLabel->labelsize(11);
	pkLicenseGroup->end();
	pkTabsGroup->end();

	// Close
	Fl_Pack* pkButtonGroup = new Fl_Pack(0, 0, 330, 20);
	pkButtonGroup->type(Fl_Pack::HORIZONTAL);
	pkButtonGroup->spacing(5);
	Fl_Box* pkButtonFiller = new Fl_Box(0, 0, 250, 20);
	Fl_Button* pkCloseButton = new Fl_Button(0, 0, 75, 20, "Close");
	pkCloseButton->labelsize(11);
	pkCloseButton->type(FL_NORMAL_BUTTON);
	pkCloseButton->shortcut(FL_Escape);
	pkCloseButton->callback((Fl_Callback *)close_window, this);
	pkButtonGroup->end();

	pkMainGroup->end();

	// Window
	xclass("iola");
	callback((Fl_Callback *)close_window, this);
	set_modal();
	end();
}

About::~About()
{
}

void About::close_window()
{
	hide();
}

} // namespace gui
} // namespace iola
