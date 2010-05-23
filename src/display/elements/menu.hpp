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

#ifndef _DISPLAY_ELEMENTS_MENU_HPP_
#define _DISPLAY_ELEMENTS_MENU_HPP_

#include <string>
#include <vector>

namespace bombherman
{
	namespace display
	{
		namespace elements
		{
			class Menu
			{
			public:
				/// Type of the menu
				typedef enum
				{
					MAIN = 0, /**< main menu */
					SETTINGS, /**< settings menu */
					GAME, /**< game menu */
				} Type;
				
				/// For the display backend to get the menu
				/**
				 * @param type The type of the menu to get
				 * 
				 * @return A menu
				 */
				static Menu * getMenu(Type type);
				
				/// To get the content of the menu
				/**
				 * @return The vector of the content of the menu
				 */
				std::vector<std::string> getContent() { return this->content; }
				/// To get the current item of the menu
				/**
				 * @return The place of the current item of the menu
				 */
				unsigned int getCurrent() { return this->current; }
				
				/// Up in the menu
				/**
				 * @return The new place in the menu
				 */
				unsigned int up();
				/// Down in the menu
				/**
				 * @return The new place in the menu
				 */
				unsigned int down();
				/// Make the action
				void action();
				
				/// Destructor
				virtual ~Menu() {}
			
			private:
				Menu(Type type, std::vector<std::string> content) : type(type), content(content), current(1) {}
				
				Type type;
				std::vector<std::string> content;
				unsigned int current;
			};
		}
	}
}



#endif // _DISPLAY_ELEMENTS_MENU_HPP_
