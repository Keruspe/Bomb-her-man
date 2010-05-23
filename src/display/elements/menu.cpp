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

#include "bombherman.hpp"
#include "game/game.hpp"
#include "menu.hpp"

using namespace bombherman;
using namespace bombherman::display;
using namespace bombherman::display::elements;

Menu *
Menu::getMenu(Type which)
{
	std::vector<std::string> menu;
	switch ( which )
	{
		case MAIN:
			menu.push_back(_("Main menu"));
			menu.push_back(_("Play"));
			menu.push_back(_("Settings"));
			menu.push_back(_("Quit"));
		break;
		case SETTINGS:
			menu.push_back(_("Settings"));
			menu.push_back(_("Back"));
		break;
		case GAME:
			menu.push_back(_("Game"));
			menu.push_back(_("Play"));
			menu.push_back(_("Back"));
		break;
	}
	return new Menu(which, menu);
}

void
Menu::action()
{
	switch ( this->type )
	{
		case MAIN:
			switch ( this->actual )
			{
				case 1:
					Game::changeMenu(GAME);
				break;
				case 2:
					Game::changeMenu(SETTINGS);
				break;
				default:
					Game::stop();
			}
		break;
		case SETTINGS:
			switch ( this->actual )
			{
				default:
					Game::changeMenu(MAIN);
			}
		break;
		case GAME:
			switch ( this->actual )
			{
				case 1:
					Game::play();
				break;
				default:
					Game::changeMenu(MAIN);
			}
		break;
	}
}

unsigned int
Menu::up()
{
	if ( --(this->actual) < 1 )
		this->actual = 1;
	
	return this->actual;
}

unsigned int
Menu::down()
{
	if ( ++(this->actual) == content.size() )
		this->actual = ( content.size() - 1 );
	
	return this->actual;
}
