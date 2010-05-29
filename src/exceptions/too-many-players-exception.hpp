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

#ifndef _TOO_MANY_PLAYERS_EXCEPTION_HPP_
#define _TOO_MANY_PLAYERS_EXCEPTION_HPP_

#include "exceptions/exception.hpp"

namespace bombherman
{
	namespace exceptions
	{
		class TooManyPlayersException :
			public Exception 
		{
		public:
			TooManyPlayersException() throw() :
				Exception("Too many player creations have been called, you have reached the maximum number allowed.") {}
			virtual ~TooManyPlayersException() throw () {}
		};
	}
}

#endif // _TOO_MANY_PLAYERS_EXCEPTION_HPP_

