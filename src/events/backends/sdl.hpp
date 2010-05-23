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

#ifndef _EVENTS_BACKENDS_SDL_HPP_
#define _EVENTS_BACKENDS_SDL_HPP_

#include "events/backends/backend.hpp"
#include "exceptions/events/nosdl-exception.hpp"

#include <sdlmm.h>

namespace bombherman
{
	namespace events
	{
		namespace backends
		{
			class SDL :
				public EventsBackend
			{
			public:
				/// Constructor
				/**
				 * @param players number of players (to create corresponding threads)
				 */
				SDL(int players = 1);
				/// Destructor
				virtual ~SDL();
			
			private:
			};
		}
	}
}

#endif // _EVENTS_BACKENDS_SDL_HPP_
