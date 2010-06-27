/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

// Initialize statics
std::map< Menu::Type, Menu * > Menu::menus;

Menu::Menu(Type type) :
	type(type),
	content(),
	current(1)
{
	// Fill the menu when creating it
	this->setContent();
}

void
Menu::setContent()
{
	this->content.clear();

	// Determine the type of menu we want to access and fill the menu
	switch ( this->type )
	{
	case MAIN:
		this->content.push_back(_("Main menu"));
		this->content.push_back(_("Play"));
		this->content.push_back(_("Settings"));
		this->content.push_back(_("Quit"));
	break;
	case SETTINGS:
		this->content.push_back(_("Settings"));
		this->content.push_back(( Config::getInt("fullscreen") ) ? _("Go to Windowed") : _("Go to Fullscreen"));
		//this->content.push_back(_("Maximum number of players: ") + Config::get("maxPlayers"));
		this->content.push_back(_("Players: ") + Config::get("nbPlayers"));
		this->content.push_back(_("AIs: ") + Config::get("nbAIs"));
		this->content.push_back(_("Maps: ") + Config::get("nbMaps"));
		this->content.push_back(_("Back"));
	break;
	}
}

Menu *
Menu::getMenu(Type which)
{
	// Kind of map of singletons (1 for each type)
	Menu * menu = Menu::menus[which];
	if ( ! menu )
		menu = new Menu(which);
	return menu;
}

void
Menu::clear()
{
	// Delete the menus from the map
	for ( std::map< Menu::Type, Menu * >::iterator i = Menu::menus.begin(),
		e = Menu::menus.end(); i != e ; ++i )
			delete(i->second);
}

void
Menu::action()
{
	int n;
	
	// Do we have to update the display ?
	bool update = false;
	
	// Manage the different kind of menus
	switch ( this->type )
	{
	case MAIN:
		switch ( this->current )
		{
		case 1: // Play
			Game::newGame();
			break;
		case 2: // Settings
			Game::changeMenu(SETTINGS);
			break;
		default: // Quit
			Game::stop();
			break;
		}
	break;
	case SETTINGS:
		switch ( this->current )
		{
		case 1: // Fullscreen / Windowed
			n = ( Config::getInt("fullscreen") + 1 ) % 2;
			Config::set("fullscreen", n);
			Display::changeFullscreen();
			update = true;
		break;
		//case 2: // Maximum number of players
		//	Config::set("maxPlayers", 2);
		//	update = true;
		//break;
		case 2: // Players
		case 3: // AIs
		case 4: // Maps
			break;
		default: // Back
			Game::changeMenu(MAIN);
		break;
		}
	break;
	}

	if ( update )
	{
		// Update the display if we need
		this->setContent();
		Display::displayMenu(this);
	}
}

void
Menu::quit()
{
	switch ( this->type )
	{
	case MAIN: // Exit the application
		Game::stop();
	break;
	default: // Go back to main menu
		Game::changeMenu(MAIN);
	break;
	}
}

void
Menu::up()
{
	if ( --(this->current) < 1 )
		// We cannot go upper than the first element
		this->current = 1;
	Display::displayMenu(this);
}

void
Menu::down()
{
	if ( ++(this->current) == content.size() )
		// We cannot go lower than the last element
		this->current = ( content.size() - 1 );
	Display::displayMenu(this);
}

void
Menu::left()
{
	int n;
	
	// We'll probably have to update to the new value if it changes
	bool update = true;
	
	// Detect the kind of menu we're in
	switch ( this->type )
	{
	case SETTINGS:
		switch ( this->current )
		{
		case 1: // Fullscreen / Windowed
			n = ( Config::getInt("fullscreen") + 1 ) % 2;
			Config::set("fullscreen", n);
			Display::changeFullscreen();
		break;
		/*
		case 2: // Maximum number of players
			n = Config::getInt("maxPlayers") - 1;
			if ( n < 2 )
				update = false;
			else
				Config::set("maxPlayers", n);
		break;
		*/
		case 2: // Players
			n = Config::getInt("nbPlayers") - 1;
			if ( n > 0 ) // There is at least 1 player
			{
				Config::set("nbPlayers", n);
				if ( n == 1 && Config::getInt("nbAIs") == 0 )
					// If only 1 player, we need 1 AI
					Config::set("nbAIs", 1);
			}
		break;
		case 3: // AIs
			n = Config::getInt("nbAIs") - 1;
			if ( n >= 0 )
			{
				if ( n == 0 && Config::getInt("nbPlayers") == 1 )
					// We cannot have 1 player and 0 AI
					Config::set("nbPlayers", 2);
				Config::set("nbAIs", n);
			}
		break;
		case 4: // Maps
			n = Config::getInt("nbMaps") - 1;
			if ( n > 0 )
				// At least 1 map
				Config::set("nbMaps", n);
		break;
		default: // Back
			update = false;
		break;
		}
	break;
	default:
		update = false;
	break;
	}

	if ( update )
	{
		// If we need to update display, do so
		this->setContent();
		Display::displayMenu(this);
	}
}

void
Menu::right()
{
	// see Menu::left() for comments
	int n, m;
	bool update = true;
	switch ( this->type )
	{
	case SETTINGS:
		switch ( this->current )
		{
		case 1:
			n = ( Config::getInt("fullscreen") + 1 ) % 2;
			Config::set("fullscreen", n);
			Display::changeFullscreen();
		break;
		/*
		case 2:
			Config::set("maxPlayers", Config::getInt("maxPlayers") + 1);
			break;
		*/
		case 2:
			n = Config::getInt("nbPlayers") + 1;
			if ( n <= 2 )
			{
				Config::set("nbPlayers", n);
				m = Config::getInt("maxPlayers");
				if ( Config::getInt("nbAIs") + n > m )
					Config::set("nbAIs", m - n);
			}
		break;
		case 3:
			n = Config::getInt("nbAIs") + 1;
			m = Config::getInt("maxPlayers");
			if ( n < m ) // Keep at least 1 human
			{
				Config::set("nbAIs", n);
				if ( ( Config::getInt("nbPlayers") + n ) > m )
					Config::set("nbPlayers", m - n);
			}
		break;
		case 4:
			n = Config::getInt("nbMaps") + 1;
			if ( n <= Config::getInt("maxMaps") )
				Config::set("nbMaps", n);
		break;
		default:
			update = false;
		break;
		}
	break;
	default:
		update = false;
	break;
	}

	if ( update )
	{
		this->setContent();
		Display::displayMenu(this);
	}
}

