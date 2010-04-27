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

#ifndef _DISPLAY_BACKENDS_SDL_HPP_
#define _DISPLAY_BACKENDS_SDL_HPP_

#include "backend.hpp"

#include <sdlmm.h>
#include <SDL_ttf.h>

namespace bombherman
{
	namespace display
	{
		namespace backends
		{
			class SDL :
				public Backend
			{
			public:
				SDL();
				~SDL();
				void displayMenu(elements::Menu::Type type);
				void displayMap();
			
			private:
				SDLmm::Display *sDisplay;
				SDL_Color textColor;
				int width, height;
				TTF_Font *fontTitle;
				TTF_Font *fontNormal;
			};
		}
	}
}

#endif // _DISPLAY_BACKENDS_SDL_HPP_ 
