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

#include "events/backends/backend.hpp"

#ifdef HAVE_SDLMM
#include "events/backends/sdl.hpp"
#endif

#ifdef HAVE_NCURSES
#include "events/backends/ncurses.hpp"
#endif

#ifdef HAVE_READLINE
#include "events/backends/readline.hpp"
#endif

#include "bombherman.hpp"

namespace bombherman
{
	namespace events
	{
		class Events
		{
		public:
			/// Constructor
			/**
			 * The constructor initialize the events backend
			 */
			Events();
			/// Main loop
			/**
			 * It waits for events and send it to the concerned part
			 */
			void main();
		
		private:
			/// To store the backend
			EventsBackend *bBackend;
		};
	}
}


#endif // _EVENTS_HPP_
