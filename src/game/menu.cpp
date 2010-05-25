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

#include <cstdio>

using namespace bombherman;

Menu *
Menu::getMenu(Type which)
{
	std::vector<std::string> menu;
	char *nbPlayers = NULL;
	char *nbMaps = NULL;
	switch ( which )
	{
		case MAIN:
			menu.push_back(_("Main menu"));
			menu.push_back(_("Play"));
			menu.push_back(_("Settings"));
			menu.push_back(_("Quit"));
		break;
		case GAME:
			menu.push_back(_("Game"));
			menu.push_back(_("Play"));
			menu.push_back(nbPlayers);
			menu.push_back(nbMaps);
			menu.push_back(_("Back"));
		break;
		case SETTINGS:
			menu.push_back(_("Settings"));
			menu.push_back(( Config::getInt("fullscreen") ) ? _("Fullscreen") : _("Windowed"));
			menu.push_back(_("Back"));
		break;
		case INGAME:
			menu.push_back(_("Pause"));
			menu.push_back(_("Back to game"));
			menu.push_back(_("Back to main menu"));
		break;
	}
	if ( nbPlayers )
		free(nbPlayers);
	if ( nbMaps )
		free(nbMaps);
	return new Menu(which, menu);
}

void
Menu::action()
{
	int n;
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
					Player::newPlayer();
					Player::newPlayer();
					Game::play();
				break;
				case 2:
				case 3:
				break;
				default:
					Game::changeMenu(MAIN);
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 1;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->content[1] = ( n ) ? _("Fullscreen") : _("Windowed");
					Display::displayMenu(this->content, this->current);
				break;
				default:
					Game::changeMenu(MAIN);
			}
		break;
		case INGAME:
			switch ( this->current )
			{
				case 1:
					Game::play(false);
				break;
				default:
					Game::changeMenu(MAIN, false);
			}
		break;
	}
}

unsigned int
Menu::up()
{
	if ( --(this->current) < 1 )
		this->current = 1;
	
	return this->current;
}

unsigned int
Menu::down()
{
	if ( ++(this->current) == content.size() )
		this->current = ( content.size() - 1 );
	
	return this->current;
}

void
Menu::left()
{
	int n;
	switch ( this->type )
	{
		case MAIN:
		break;
		case GAME:
			switch ( this->current )
			{
				case 1:
				break;
				default:
				break;
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 1;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->content[1] = ( n ) ? _("Fullscreen") : _("Windowed");
					Display::displayMenu(this->content, this->current);
				break;
				default:
				break;
			}
		break;
		case INGAME:
		break;
	}
}

void
Menu::right()
{
	int n;
	switch ( this->type )
	{
		case MAIN:
		break;
		case GAME:
			switch ( this->current )
			{
				case 1:
				break;
				default:
				break;
			}
		break;
		case SETTINGS:
			switch ( this->current )
			{
				case 1:
					n = ( Config::getInt("fullscreen") + 1 ) % 1;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->content[1] = ( n ) ? _("Fullscreen") : _("Windowed");
					Display::displayMenu(this->content, this->current);
				break;
				default:
				break;
			}
		break;
		case INGAME:
		break;
	}
}
