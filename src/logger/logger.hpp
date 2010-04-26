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

#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string>
#include <iostream>
#include <fstream>

namespace bombherman
{
	class Logger
	{
	public:
		/// Log a message
		/**
		 * Simply write the message in the log
		 */
		static bool putLine(std::string, bool error = true);
	
	private:
		/// Constructor
		Logger() {}
		Logger(std::string file);
		
		/// Get the correct Logger
		/**
		 * This method permit to get the correct
		 * logger (for normal or error messages)
		 */
		static Logger *getLogger(bool error = true);
		
		/// Destructor
		virtual ~Logger();
		
		
		/// Store the normal Logger
		static Logger *lLogger;
		/// Store the error logger
		static Logger *lErrorLogger;
		/// Store the file stream of the Logger
		std::fstream fLogFile;
	};
}



#endif // _LOGGER_HPP_ 
