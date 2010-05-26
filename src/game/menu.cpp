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
#include "menu.hpp"
#include "game.hpp"
#include "player.hpp"

using namespace bombherman;

std::map<Menu::Type, Menu *> Menu::menus;

Menu::Menu(Type type) :
	type(type),
	content(),
	current(1)
{
	this->setContent();
}

void
Menu::setContent()
{
	this->content.clear();
	switch ( this->type )
	{
		case MAIN:
			this->content.push_back(_("Main menu"));
			this->content.push_back(_("Play"));
			this->content.push_back(_("Settings"));
			this->content.push_back(_("Quit"));
		break;
		case GAME:
			this->content.push_back(_("Game"));
			this->content.push_back(_("Play"));
			this->content.push_back(_("Players: ") + Config::get("nbPlayers"));
			this->content.push_back(_("AIs: ") + Config::get("nbAIs"));
			this->content.push_back(_("Maps: ") + Config::get("nbMaps"));
			this->content.push_back(_("Back"));
		break;
		case SETTINGS:
			this->content.push_back(_("Settings"));
			this->content.push_back(std::string(_("Go to ")) + ( ( Config::getInt("fullscreen") ) ? _("Windowed") : _("Fullscreen") ));
			this->content.push_back(_("Maximum number of players: ") + Config::get("maxPlayers"));
			this->content.push_back(_("Back"));
		break;
		case INGAME:
			this->content.push_back(_("Pause"));
			this->content.push_back(_("Back to game"));
			this->content.push_back(_("Back to main menu"));
		break;
	}
}

Menu *
Menu::getMenu(Type which)
{
	Menu *menu = menus[which];
	if ( ! menu )
		menu = new Menu(which);
	return menu;
}

void
Menu::clear()
{
	for ( std::map<Menu::Type, Menu *>::iterator i = menus.begin(), e = menus.end(); i != e ; ++ i )
		delete(i->second);
}

void
Menu::action()
{
	int n;
	bool update = false;
	switch ( this->type )
	{
		case MAIN:
			switch ( this->current )
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
		case GAME:
			switch ( this->current )
			{
				case 1:
					Game::newGame();
				break;
				case 2:
				case 3:
				case 4:
				break;
				default:
					Game::changeMenu(MAIN);
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					update = true;
				break;
				case 2:
					Config::set("maxPlayers", 2);
					update = true;
				break;
				default:
					Game::changeMenu(MAIN);
			}
		break;
		case INGAME:
			switch ( this->current )
			{
				case 1:
					Game::play();
				break;
				default:
					Game::changeMenu(MAIN, false);
			}
		break;
	}
	if ( update )
	{
		this->setContent();
		Display::displayMenu(this);
	}
}

void
Menu::up()
{
	if ( --(this->current) < 1 )
		this->current = 1;
	
	Display::displayMenu(this);
}

void
Menu::down()
{
	if ( ++(this->current) == content.size() )
		this->current = ( content.size() - 1 );
	
	Display::displayMenu(this);
}

void
Menu::left()
{
	int n;
	bool update = true;
	switch ( this->type )
	{
		case MAIN:
			update = false;
		break;
		case GAME:
			switch ( this->current )
			{
				case 2:
					n = Config::getInt("nbPlayers") - 1;
					if ( n > 0 )
					{
						Config::set("nbPlayers", n);
						if ( ( n == 1 ) && ( Config::getInt("nbAIs") == 0 ) )
							Config::set("nbAIs", 1);
					}
				break;
				case 3:
					n = Config::getInt("nbAIs") - 1;
					if ( n >= 0 )
					{
						if ( ( n == 0 ) && ( Config::getInt("nbPlayers") == 1 ) )
							Config::set("nbAIs", 1);
						else
							Config::set("nbAIs", n);
					}
				break;
				case 4:
					n = Config::getInt("nbMaps") - 1;
					if ( n > 0 )
					{
						Config::set("nbMaps", n);
					}
				break;
				default:
					update = false;
				break;
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
				break;
				case 2:
					n = Config::getInt("maxPlayers") - 1;
					if ( n < 2 )
						update = false;
					else
						Config::set("maxPlayers", n);
				break;
				default:
					update = false;
				break;
			}
		break;
		case INGAME:
			update = false;
		break;
	}
	if ( update )
	{
		this->setContent();
		Display::displayMenu(this);
	}
}

void
Menu::right()
{
	int n, m;
	bool update = true;
	switch ( this->type )
	{
		case MAIN:
			update = false;
		break;
		case GAME:
			switch ( this->current )
			{
				case 2:
					n = Config::getInt("nbPlayers") + 1;
					if ( n <= 2 )
					{
						Config::set("nbPlayers", n);
						m = Config::getInt("maxPlayers");
						if ( ( Config::getInt("nbAIs") + n ) > m )
							Config::set("nbAIs", m - n);
					}
				break;
				case 3:
					n = Config::getInt("nbAIs") + 1;
					if ( n <= Config::getInt("maxPlayers") )
					{
						Config::set("nbAIs", n);
						m = Config::getInt("maxPlayers");
						if ( ( Config::getInt("nbPlayers") + n ) > m )
							Config::set("nbPlayers", m - n);
					}
				break;
				case 4:
					n = Config::getInt("nbMaps") + 1;
					if ( n <= Config::getInt("maxMaps") )
					{
						Config::set("nbMaps", n);
					}
				break;
				default:
					update = false;
				break;
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
				break;
				case 2:
						Config::set("maxPlayers", Config::getInt("maxPlayers") + 1);
				break;
				default:
					update = false;
				break;
			}
		break;
		case INGAME:
			update = false;
		break;
	}
	if ( update )
	{
		this->setContent();
		Display::displayMenu(this);
	}
}
