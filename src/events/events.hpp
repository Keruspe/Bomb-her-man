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

#ifndef _EVENTS_HPP_
#define _EVENTS_HPP_

#include "bombherman.hpp"

#include "exceptions/events/nosdl-exception.hpp"

#include <SDL.h>

typedef void (*bombherman_waitquit) ();

namespace bombherman
{
	namespace events
	{
		class Events
		{
		public:
			/// Constructor
			/**
			 * @param players number of players (to create corresponding threads)
			 */
			Events(int players = 1);
			
			/// Listen to events
			bombherman_waitquit *listen();
			
			/// Destructor
			virtual ~Events();
		
		private:
		};
	}
}

#endif // _EVENTS_HPP_
