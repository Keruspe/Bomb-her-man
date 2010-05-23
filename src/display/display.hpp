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

#include "bombherman.hpp"

#include "display/elements/menu.hpp"

#include "exceptions/display/nosdl-exception.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

namespace bombherman
{
	namespace display
	{
		class Display
		{
		public:
			/// Constructor
			Display();
			/// Destructor
			virtual ~Display();
			/// Display the menu
			/**
			 * @param type The type of the menu to display
			 */
			void displayMenu(elements::Menu::Type type);
			/// Display the map
			void displayMap();
		
		private:
			/// To store the SDL display surface
			SDL_Surface *sDisplay;
			/// To store the SDL text color
			SDL_Color textColor;
			/// Width and Height of the display
			int width, height;
			/// To store the SDL title font
			TTF_Font *fontTitle;
			/// To store the SDL normal font
			TTF_Font *fontNormal;
		};
	}
}

#endif // _DISPLAY_HPP_
