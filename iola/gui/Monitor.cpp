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

// STD
#include <cassert>
#include <stdexcept>

// RLOG
#include <rlog/rlog.h>

// QT
#include <QtGui>

// IOLA
#include "Monitor.h"

namespace iola
{
namespace gui
{

Monitor::Monitor(QWidget* parent) :
	QGLWidget(parent),
	m_pkConsumer(0)
{
	setAutoBufferSwap(false);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Monitor::connect_to(Mlt::Consumer* consumer)
{
	if (!consumer)
		throw std::invalid_argument("Consumer passed as parameter is NULL");

	m_pkConsumer = consumer;
}

void Monitor::stop()
{
	assert(m_pkConsumer);
	rDebug("%s: Stopping consumer", __PRETTY_FUNCTION__);
	m_pkConsumer->stop();
}

bool Monitor::restart()
{
	assert(m_pkConsumer);
	bool ret = m_pkConsumer->is_stopped();
	if (ret)
	{
		rDebug("%s: Starting consumer", __PRETTY_FUNCTION__);
		m_pkConsumer->set("display", this, NULL);
		m_pkConsumer->start();
	}
	if (!m_pkConsumer->is_stopped())
		refresh();
	return ret;
}

void Monitor::refresh()
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("refresh", 1);
	m_pkConsumer->unlock();
}

void Monitor::resizeEvent(QResizeEvent* event)
{
	assert(m_pkConsumer);
	if (!m_pkConsumer)
		return;

	doneCurrent();
	m_pkConsumer->lock();
	m_pkConsumer->set("window_width", event->size().width());
	m_pkConsumer->set("window_height", event->size().height());
	m_pkConsumer->unlock();
}

void Monitor::paintEvent(QPaintEvent*)
{
	//NOTE Handled by consumer
}

} // namespace gui
} // namespace iola
