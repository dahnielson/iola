/* -*- Mode: C++ ; indent-tabs-mode: t ; c-basic-offset: 8 -*- */
#ifndef IOLA_GUI_MONITOR_H
#define IOLA_GUI_MONITOR_H

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

// MLT
#include <mlt++/Mlt.h>

// QT
#include <QGLWidget>

namespace iola
{
namespace gui
{

////////////////////////////////////////////////////////////////////////////
// class iola::gui::Monitor

class Monitor :
    public QGLWidget
{
	Q_OBJECT

public:
	Monitor(QWidget* parent = 0);
	void connect_to(Mlt::Consumer* consumer);

	/// Stop the monitor consumer
	void stop();
	/// Restart the monitor consumer
	bool restart();
	/// Refresh the monitor consumer
	void refresh();

protected:
	void resizeEvent(QResizeEvent* event);
	void paintEvent(QPaintEvent*);

private:
	Mlt::Consumer* m_pkConsumer;
};

} // namespace gui
} // namespace iola

#endif // IOLA_GUI_MONITOR_H
