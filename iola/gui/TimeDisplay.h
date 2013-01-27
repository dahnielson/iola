/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_TIMEDISPLAY_H
#define IOLA_GUI_TIMEDISPLAY_H

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

// QT
#include <QPaintEvent>
#include <QLabel>

// IOLA
#include <iola/model/timecode.h>

namespace iola
{
namespace gui
{

////////////////////////////////////////////////////////////////////////////
// class iola::gui::TimeDisplay

/// Time display widget
class TimeDisplay :
	public QLabel
{
	Q_OBJECT

public:
	TimeDisplay(QWidget* parent = 0);

public slots:
	/// Set time base (24, 25, 30, etc.)
	void set_timebase(const int timebase);
	/// Set drop frame
	void set_dropframe(const bool dropframe);
	/// Set time in frames
	void set_framecount(const int frame);

protected:
	/// Qt paint event
	void paintEvent(QPaintEvent* event);

private:
	bool m_bDropFrame;
	iola::model::timecode m_kTimecode;
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_TIMEDISPLAY_H
