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


#include "display/elements/menu.hpp"

#include <iostream>
#include <string>

#ifdef HAVE_OPENGL
#include "backends/opengl.hpp"
#endif

#ifdef HAVE_SDLMM
#include "backends/sdl.hpp"
#endif

#ifdef HAVE_NCURSES
#include "backends/ncurses.hpp"
#endif

#include "backends/ascii.hpp"

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
			 */
			bool displayMenu(elements::MenuType type = elements::MAIN);
			
			/// Display the map
			/**
			 * For now, just call the backend
			 * related function
			 */
			bool displayMap();
		
		private:
			/// To store the backend
			backends::Backend *bBackend;
		};
	}
}

#endif // _DISPLAY_HPP_ 
