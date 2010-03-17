/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
 * 
 * Bomb-her-man is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Bomb-her-man is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logger.hpp"

using namespace bombherman;
using namespace bombherman::logger;


Logger *
Logger::getLogger(bool error)
{
	if ( error )
	{
		if ( ! lErrorLogger )
			lErrorLogger = new Logger("/tmp/bomb-her-man.log");
		return lErrorLogger;
	}
	else
	{
		if ( ! lLogger )
			lLogger = new Logger("/tmp/bomb-her-man.err");
		return lLogger;
	}
}

Logger::Logger(std::string file)
{
	fLogFile.open(file.c_str());
}

Logger::~Logger()
{
	fLogFile.close();
}

bool
Logger::putLine(std::string message)
{
	fLogFile << "[date]\t" << message << std::endl;
	return true;
}
