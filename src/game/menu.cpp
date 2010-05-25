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
			this->content.push_back(_("Maps: ") + Config::get("nbMaps"));
			this->content.push_back(_("Back"));
		break;
		case SETTINGS:
			this->content.push_back(_("Settings"));
			this->content.push_back(std::string(_("Go to ")) + ( ( Config::getInt("fullscreen") ) ? _("Windowed") : _("Fullscreen") ));
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
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->setContent();
					Display::displayMenu(this);
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
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->setContent();
					Display::displayMenu(this);
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
					n = ( Config::getInt("fullscreen") + 1 ) % 2;
					Config::set("fullscreen", n);
					Display::changeFullscreen();
					this->setContent();
					Display::displayMenu(this);
				break;
				default:
				break;
			}
		break;
		case INGAME:
		break;
	}
}
