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

// QT
#include <QtGui>

// IOLA
#include "TimeDisplay.h"

namespace iola
{
namespace gui
{

TimeDisplay::TimeDisplay(QWidget* parent) :
	QLabel(parent),
	m_bDropFrame(true),
	m_kTimecode(30)
{
	setFrameStyle(QFrame::Panel | QFrame::Sunken);
}

void TimeDisplay::set_timebase(const int timebase)
{
	m_kTimecode.set_timebase(timebase);
	repaint();
}

void TimeDisplay::set_dropframe(const bool dropframe)
{
	m_bDropFrame = dropframe;
	repaint();
}

void TimeDisplay::set_framecount(const int frame)
{
	m_kTimecode.set_framecount(frame);
	repaint();
}

void TimeDisplay::paintEvent(QPaintEvent* event)
{
	const char* time = m_kTimecode.get_timecode(m_bDropFrame).c_str();
	setText(QString(time));
	QLabel::paintEvent(event);
}

} // namespace gui
} // namespace iola
