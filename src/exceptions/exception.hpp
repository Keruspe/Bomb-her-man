/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <string>

namespace bombherman
{
	namespace exceptions
	{
		class Exception :
			public std::exception
		{
		public:
			/// Destructor
			virtual ~Exception() throw();

			/// Give you the message of the exception
			/**
			  * @return The message (std::string)
			  */
			const std::string & message() const throw();
		protected:
			///Constructor
			Exception(const std::string & message) throw();
		private:
			const std::string _message;
		};
	}
}

#endif // _EXCEPTION_HPP_

