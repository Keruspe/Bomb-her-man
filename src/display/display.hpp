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

#ifndef _DISPLAY_HPP_
#define _DISPLAY_HPP_

#include <iostream>
#include <string>

#include "display/backends/backend.hpp"

#ifdef HAVE_OPENGL
#include "display/backends/opengl.hpp"
#endif

#ifdef HAVE_SDLMM
#include "display/backends/sdl.hpp"
#endif

#ifdef HAVE_NCURSES
#include "display/backends/ncurses.hpp"
#endif

#include "display/backends/ascii.hpp"

#include "bombherman.hpp"

namespace bombherman
{
	namespace display
	{
		class Display
		{
		public:
			/// Constructor
			/**
			 * The constructor initialize the video backend
			 */
			Display();
			
			/// Destructor
			virtual ~Display();
			
			/// Display the menu
			/**
			 * For now, just call the backend
			 * related function
			 * 
			 * @param type The type of the menu to display
			 */
			void displayMenu(elements::Menu::Type type = elements::Menu::MAIN);
			
			/// Display the map
			/**
			 * For now, just call the backend
			 * related function
			 */
			void displayMap();
		
		private:
			/// To store the backend
			DisplayBackend *bBackend;
		};
	}
}

#endif // _DISPLAY_HPP_
