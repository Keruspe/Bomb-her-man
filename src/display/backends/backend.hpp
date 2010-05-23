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

#ifndef _DISPLAY_BACKENDS_BACKEND_HPP_
#define _DISPLAY_BACKENDS_BACKEND_HPP_

#include "bombherman.hpp"

#include "display/elements/menu.hpp"

namespace bombherman
{
	namespace display
	{
		class DisplayBackend
		{
		public:
			/// Display the menu
			/**
			 * @param type The type of the menu to display
			 */
			virtual void displayMenu(elements::Menu::Type type) = 0;
			/// Display the map
			virtual void displayMap() = 0;
			/// Destructor
			virtual ~DisplayBackend() {}
		};
	}
}

#endif // _DISPLAY_BACKENDS_BACKEND_HPP_
