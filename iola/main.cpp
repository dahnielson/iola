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

// STD
#include <exception>
#include <iostream>

// BOOST
#include <boost/program_options.hpp>

// RLOG
#include <rlog/rlog.h>
#include <rlog/rloginit.h>
#include <rlog/StdioNode.h>
#include <rlog/SyslogNode.h>
#include <rlog/RLogChannel.h>

// IOLA
#include <iola/application/get_instance.h>

namespace po = boost::program_options;

int main(int argc, char **argv)
{
	int iStdioDbgLevel, iSyslogDbgLevel;

	// Declare program options
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "Print usage information and exit.")
		("stdio-dbg", po::value<int>(&iStdioDbgLevel)->default_value(0),
		 "Set debug level for stdio.")
		("syslog-dbg", po::value<int>(&iSyslogDbgLevel)->default_value(0),
		 "Set debug level for syslog.")
		;

	// Parse commandline
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// Print help
	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 1;
	}

	// Initalize logging
	rlog::RLogInit(argc, argv);

	// Set up log to syslog
	rlog::SyslogNode syslog("iola");
	if (iSyslogDbgLevel > 0) syslog.subscribeTo(rlog::GetGlobalChannel("error"));
	if (iSyslogDbgLevel > 1) syslog.subscribeTo(rlog::GetGlobalChannel("warning"));
	if (iSyslogDbgLevel > 2) syslog.subscribeTo(rlog::GetGlobalChannel("debug"));

	// Set up log to standard out
	rlog::StdioNode stdlog(STDERR_FILENO, rlog::StdioNode::OutputColor);
	if (iStdioDbgLevel > 0) stdlog.subscribeTo(rlog::GetGlobalChannel("error"));
	if (iStdioDbgLevel > 1) stdlog.subscribeTo(rlog::GetGlobalChannel("warning"));
	if (iStdioDbgLevel > 2) stdlog.subscribeTo(rlog::GetGlobalChannel("debug"));
	if (iStdioDbgLevel > 3) stdlog.subscribeTo(rlog::GetGlobalChannel("info"));

	try
	{
		iola::application::iapplication* const application = iola::application::get_instance();
		application->run();
	}
	catch (std::exception& e)
	{
		rError("%s: Caught exception: %s", __PRETTY_FUNCTION__, e.what());
		throw;
	}
	catch (...)
	{
		rError("%s: Caught unknown exception ... terminating", __PRETTY_FUNCTION__);
	}

	return 0;
}
