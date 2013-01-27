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
#include <exception>

// RLOG
#include <rlog/rlog.h>

// QT
#include <QtGui>

// IOLA
#include <iola/model/iprogram.h>
#include "Monitor.h"
#include "ProgramMonitor.h"
#include "TimeDisplay.h"

namespace iola
{
namespace gui
{

ProgramMonitor::ProgramMonitor(QWidget* parent) :
	QFrame(parent),
	m_pkMonitor(0),
	m_pkCurrentTime(0),
	m_pkDurationTime(0),
	m_pkModel(0),
	m_pkProgram(0),
	m_pkConsumer(0),
	m_pkFrameShowEvent(0)
{
	// Consumer
	char consumer_id[] = "iola"; //NOTE Using our custom consumer
	Mlt::Profile kProfile; //NOTE The MLT default profile
	m_pkConsumer = Mlt::Factory::consumer(kProfile, consumer_id);
	assert(m_pkConsumer->get_consumer());

	m_pkConsumer->lock();
	m_pkFrameShowEvent = m_pkConsumer->listen("consumer-frame-show", this, (mlt_listener)frame_show_callback);
	m_pkConsumer->unlock();
	assert(m_pkFrameShowEvent);

	// UI
	m_pkMonitor = new Monitor();
	m_pkMonitor->connect_to(m_pkConsumer);

	QHBoxLayout* pkMonitorLayout = new QHBoxLayout;
	pkMonitorLayout->addWidget(m_pkMonitor);

	m_pkCurrentTime = new TimeDisplay;
	m_pkDurationTime = new TimeDisplay;

	QPushButton* pkPrevEdit = new QPushButton;
	QPushButton* pkGotoIn = new QPushButton;
	QPushButton* pkClearIn = new QPushButton;
	QPushButton* pkMarkIn = new QPushButton;
	QPushButton* pkPlayBackward = new QPushButton;
	QPushButton* pkStepBackward = new QPushButton;
	QPushButton* pkStop = new QPushButton;
	QPushButton* pkStepForward = new QPushButton;
	QPushButton* pkPlayForward = new QPushButton;
	QPushButton* pkMarkOut = new QPushButton;
	QPushButton* pkClearOut = new QPushButton;
	QPushButton* pkGotoOut = new QPushButton;
	QPushButton* pkNextEdit = new QPushButton;

	pkPrevEdit->setIcon(QIcon(":/xbm/goto_prev_edit.xbm"));
	pkGotoIn->setIcon(QIcon(":/xbm/goto_mark_in.xbm"));
	pkClearIn->setIcon(QIcon(":/xbm/clear_in.xbm"));
	pkMarkIn->setIcon(QIcon(":/xbm/mark_in.xbm"));
	pkPlayBackward->setIcon(QIcon(":/xbm/play_rev.xbm"));
	pkStepBackward->setIcon(QIcon(":/xbm/step_rev.xbm"));
	pkStop->setIcon(QIcon(":/xbm/pause.xbm"));
	pkStepForward->setIcon(QIcon(":/xbm/step_fwd.xbm"));
	pkPlayForward->setIcon(QIcon(":/xbm/play_fwd.xbm"));
	pkMarkOut->setIcon(QIcon(":/xbm/mark_out.xbm"));
	pkClearOut->setIcon(QIcon(":/xbm/clear_out.xbm"));
	pkGotoOut->setIcon(QIcon(":/xbm/goto_mark_out.xbm"));
	pkNextEdit->setIcon(QIcon(":/xbm/goto_next_edit.xbm"));

	connect(pkPrevEdit, SIGNAL(clicked()), this, SLOT(goto_previous_edit()));
	connect(pkGotoIn, SIGNAL(clicked()), this, SLOT(goto_mark_in()));
	connect(pkClearIn, SIGNAL(clicked()), this, SLOT(clear_mark_in()));
	connect(pkMarkIn, SIGNAL(clicked()), this, SLOT(mark_in()));
	connect(pkPlayBackward, SIGNAL(clicked()), this, SLOT(play_backward()));
	connect(pkStepBackward, SIGNAL(clicked()), this, SLOT(step_backward()));
	connect(pkStop, SIGNAL(clicked()), this, SLOT(stop_playback()));
	connect(pkStepForward, SIGNAL(clicked()), this, SLOT(step_forward()));
	connect(pkPlayForward, SIGNAL(clicked()), this, SLOT(play_forward()));
	connect(pkMarkOut, SIGNAL(clicked()), this, SLOT(mark_out()));
	connect(pkClearOut, SIGNAL(clicked()), this, SLOT(clear_mark_out()));
	connect(pkGotoOut, SIGNAL(clicked()), this, SLOT(goto_mark_out()));
	connect(pkNextEdit, SIGNAL(clicked()), this, SLOT(goto_next_edit()));

	QHBoxLayout* pkTransportButtonLayout = new QHBoxLayout;
	pkTransportButtonLayout->setSpacing(0);
	pkTransportButtonLayout->addWidget(m_pkCurrentTime);
	pkTransportButtonLayout->addStretch();
	pkTransportButtonLayout->addWidget(pkPrevEdit);
	pkTransportButtonLayout->addWidget(pkGotoIn);
	pkTransportButtonLayout->addWidget(pkClearIn);
	pkTransportButtonLayout->addWidget(pkMarkIn);
	pkTransportButtonLayout->addWidget(pkPlayBackward);
	pkTransportButtonLayout->addWidget(pkStepBackward);
	pkTransportButtonLayout->addWidget(pkStop);
	pkTransportButtonLayout->addWidget(pkStepForward);
	pkTransportButtonLayout->addWidget(pkPlayForward);
	pkTransportButtonLayout->addWidget(pkMarkOut);
	pkTransportButtonLayout->addWidget(pkClearOut);
	pkTransportButtonLayout->addWidget(pkGotoOut);
	pkTransportButtonLayout->addWidget(pkNextEdit);
	pkTransportButtonLayout->addStretch();
	pkTransportButtonLayout->addWidget(m_pkDurationTime);

	QVBoxLayout* pkLayout = new QVBoxLayout;
	pkLayout->addLayout(pkMonitorLayout);
	pkLayout->addLayout(pkTransportButtonLayout);

	setLayout(pkLayout);
	setFrameStyle(QFrame::Panel | QFrame::Raised);

	rDebug("%s: Program monitor constructed", __PRETTY_FUNCTION__);
}

ProgramMonitor::~ProgramMonitor()
{
	assert(m_pkFrameShowEvent);
	delete m_pkFrameShowEvent;

	// Video settings change signals
	on_width_change_connection.disconnect();
	on_height_change_connection.disconnect();
	on_dar_change_connection.disconnect();
	on_par_change_connection.disconnect();
	on_progressive_change_connection.disconnect();
	on_fps_change_connection.disconnect();
	on_timebase_change_connection.disconnect();
	on_ntsc_change_connection.disconnect();

	// Audio settings change signals
	on_sample_depth_change_connection.disconnect();
	on_sample_rate_change_connection.disconnect();

	// Markers change signal
	on_marks_change_connection.disconnect();

	// Source change signals
	on_playback_connection.disconnect();
	on_producer_change_connection.disconnect();

	// Consumer
	assert(m_pkConsumer);
	m_pkConsumer->stop();
	delete m_pkConsumer;

	rDebug("%s: Program monitor destructed", __PRETTY_FUNCTION__);
}

void ProgramMonitor::connect_to(iola::model::imodel* model)
{
	if (!model)
		throw std::invalid_argument("Model passed as parameter is NULL");

	m_pkModel = model;
	m_pkProgram = model->program();

	assert(m_pkProgram);
	assert(m_pkConsumer);
	m_pkProgram->connect_to(m_pkConsumer);

	assert(m_pkMonitor);
	m_pkMonitor->restart();

	// Video settings change signals
	on_width_change_connection = m_pkProgram->on_width_change_signal.connect(
		boost::bind(&ProgramMonitor::set_width, this, _1)
		);
	on_height_change_connection = m_pkProgram->on_height_change_signal.connect(
		boost::bind(&ProgramMonitor::set_height, this, _1)
		);
	on_dar_change_connection = m_pkProgram->on_dar_change_signal.connect(
		boost::bind(&ProgramMonitor::set_dar, this, _1)
		);
	on_par_change_connection = m_pkProgram->on_par_change_signal.connect(
		boost::bind(&ProgramMonitor::set_par, this, _1)
		);
	on_fps_change_connection = m_pkProgram->on_fps_change_signal.connect(
		boost::bind(&ProgramMonitor::set_fps, this, _1)
		);
	on_timebase_change_connection = m_pkProgram->on_timebase_change_signal.connect(
		boost::bind(&ProgramMonitor::set_timebase, this, _1)
		);
	on_ntsc_change_connection = m_pkProgram->on_ntsc_change_signal.connect(
		boost::bind(&ProgramMonitor::set_ntsc, this, _1)
		);
	on_progressive_change_connection = m_pkProgram->on_progressive_change_signal.connect(
		boost::bind(&ProgramMonitor::set_progressive, this, _1)
		);

	// Audio settings change signals
	on_sample_depth_change_connection = m_pkProgram->on_sample_depth_change_signal.connect(
		boost::bind(&ProgramMonitor::set_sample_depth, this, _1)
		);
	on_sample_rate_change_connection = m_pkProgram->on_sample_rate_change_signal.connect(
		boost::bind(&ProgramMonitor::set_sample_rate, this, _1)
		);

	// Markers change signal
	on_marks_change_connection = m_pkProgram->on_mark_change_signal.connect(
		boost::bind(&ProgramMonitor::on_marks_change, this, _3, _4)
		);

	// Program change signals
	on_playback_connection = m_pkProgram->on_playback_change_signal.connect(
		boost::bind(&ProgramMonitor::on_playback_change, this)
		);
	on_producer_change_connection = m_pkProgram->on_producer_change_signal.connect(
		boost::bind(&ProgramMonitor::on_producer_change, this, _1, _2)
		);
	
	rDebug("%s: Program monitor connected", __PRETTY_FUNCTION__);
}

void ProgramMonitor::set_width(const int width)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("width", width);
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_height(const int height)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("height", height);
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_dar(const boost::rational<int> dar)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("display_aspect_num", dar.numerator());
	m_pkConsumer->set("display_aspect_den", dar.denominator());
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_par(const boost::rational<int> par)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("sample_aspect_num", par.numerator());
	m_pkConsumer->set("sample_aspect_den", par.denominator());
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_fps(const boost::rational<int> fps)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("frame_rate_num", fps.numerator());
	m_pkConsumer->set("frame_rate_den", fps.denominator());
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_timebase(const int timebase)
{
	assert(m_pkCurrentTime);
	assert(m_pkDurationTime);
	m_pkCurrentTime->set_timebase(timebase);
	m_pkDurationTime->set_timebase(timebase);
}

void ProgramMonitor::set_ntsc(const bool ntsc)
{
	assert(m_pkCurrentTime);
	assert(m_pkDurationTime);
	m_pkCurrentTime->set_dropframe(ntsc);
	m_pkDurationTime->set_dropframe(ntsc);
}

void ProgramMonitor::set_progressive(const bool progressive)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("progressive", progressive);
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_sample_depth(const int depth)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("audio_format", depth);
	m_pkConsumer->unlock();
}

void ProgramMonitor::set_sample_rate(const int rate)
{
	assert(m_pkConsumer);
	m_pkConsumer->lock();
	m_pkConsumer->set("frequency", rate);
	m_pkConsumer->unlock();
}

void ProgramMonitor::on_marks_change(const int in, const int out)
{
	//FIXME
}

void ProgramMonitor::on_playback_change()
{
	assert(m_pkMonitor);
	m_pkMonitor->refresh();
}

void ProgramMonitor::on_producer_change(const int start, const int end)
{
	assert(m_pkDurationTime);
	const int duration = end - start;
	m_pkDurationTime->set_framecount(duration);
}

void ProgramMonitor::frame_shown(Mlt::Frame &frame)
{
	assert(m_pkConsumer);
	assert(m_pkCurrentTime);

	if (!m_pkConsumer->is_stopped())
	{
		const int position = frame.get_int("_position");
		m_pkCurrentTime->set_framecount(position);
	}
}

void ProgramMonitor::mark_in()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->mark_in();
}

void ProgramMonitor::mark_out()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->mark_out();
}

void ProgramMonitor::mark_clip()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->mark_clip();
}

void ProgramMonitor::clear_mark_in()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->clear_mark_in();
}

void ProgramMonitor::clear_mark_out()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->clear_mark_out();
}

void ProgramMonitor::goto_mark_in()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_mark_in();
}

void ProgramMonitor::goto_mark_out()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_mark_out();
}

void ProgramMonitor::goto_start()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_start();
}

void ProgramMonitor::goto_end()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_end();
}

void ProgramMonitor::play_backward()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->play_backward();
}

void ProgramMonitor::play_forward()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->play_forward();
}

void ProgramMonitor::step_backward()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->step_backward();
}

void ProgramMonitor::step_forward()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->step_forward();
}

void ProgramMonitor::stop_playback()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->stop();
}

void ProgramMonitor::goto_previous_edit()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_previous_edit();
}

void ProgramMonitor::goto_next_edit()
{
	if (!m_pkProgram)
		return;
	m_pkProgram->goto_next_edit();
}

void ProgramMonitor::make_lift_edit()
{
	if (!m_pkModel)
		return;
	m_pkModel->make_lift_edit();
}

void ProgramMonitor::make_extract_edit()
{
	if (!m_pkModel)
		return;
	m_pkModel->make_extract_edit();
}

void ProgramMonitor::make_insert_edit()
{
	if (!m_pkModel)
		return;
	m_pkModel->make_insert_edit();
}

void ProgramMonitor::make_overwrite_edit()
{
	if (!m_pkModel)
		return;
	m_pkModel->make_overwrite_edit();
}

} // namespace gui
} // namespace iola
