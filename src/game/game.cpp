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

#include "game.hpp"
#include "menu.hpp"
#include "map/map-utils.hpp"
#include "atomic-center/atomic-center.hpp"
#include "atomic-center/bomb.hpp"
#include "AI.hpp"

using namespace bombherman;

// Initialize statics
Menu * Game::currentMenu = NULL;
std::vector< SDL_Thread * > Game::threads;
bool Game::isInit = false;
bool Game::playing = true;
Sint32 Game::mapCount = -1;

void Game::init()
{
	Display::init();
	Game::changeMenu(Menu::MAIN);
	
	// Ignore some various events
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
	SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	
	Game::isInit = true;
}

void
Game::main()
{
	if ( ! Game::isInit )
		init();
	
	SDL_Event event;
	while ( Game::playing && SDL_WaitEvent(&event) )
	{
		// Handle the events we want
		switch ( event.type )
		{
		case SDL_QUIT:
			Game::playing = false;
		break;
		case SDL_KEYDOWN:
			if ( Game::currentMenu )
				Game::eventMenu(&event.key);
			else if ( Game::mapCount < 0 )
				Game::changeMenu(Menu::MAIN);
			else
				Game::threads.push_back(SDL_CreateThread(Game::eventGame, &event.key));
		break;
		default:
		break;
		}
	}
	Game::quit();
}

void
Game::changeMenu(Menu::Type type, bool stopGame)
{
	if ( stopGame )
	{
		// If we're stopping the game, we have to clean things
		map::Map::deleteMap();
		Player::clean();
		Display::quitGame();
		bomb::AtomicCenter::boum();
	}
	// Do change the menu
	Game::currentMenu = Menu::getMenu(type);
	Display::displayMenu(currentMenu);
}

void
Game::newGame()
{
	// Create players
	for ( int i = 0, e = Config::getInt("nbPlayers") ; i < e ; ++i )
		Player::newPlayer();
	// Create AIs
	for ( int i = 0, e = Config::getInt("nbAIs") ; i < e ; ++i )
		AI::newAI();
	
	// Start the game !
	Game::mapCount = Config::getInt("nbMaps");
	Game::nextMap();
}

void
Game::nextMap()
{
	// Wait for everything to exit
	Game::waitThreads();
	map::Map::deleteMap();
	if ( --Game::mapCount < 0 )
		Display::displayScores();
	else
	{
		// We still have maps to play on !
		map::Map::newMap();
		Game::play();
	}
}

void
Game::play()
{
	// No menu there
	currentMenu = NULL;

	// Juste update the display for the map
	Display::updateMap();
}

int
Game::eventMenu(void * event)
{
	// Handle events in menus
	switch ( reinterpret_cast< SDL_KeyboardEvent * >(event)->keysym.sym )
	{
	case SDLK_ESCAPE:
		currentMenu->quit();
		break;
	case SDLK_UP:
		currentMenu->up();
		break;
	case SDLK_DOWN:
		currentMenu->down();
		break;
	case SDLK_KP_ENTER:
	case SDLK_SPACE:
	case SDLK_RETURN:
		currentMenu->action();
		break;
	case SDLK_LEFT:
		currentMenu->left();
		break;
	case SDLK_RIGHT:
		currentMenu->right();
		break;
	default:
		break;
	}
	return 0;
}

int
Game::eventGame(void * event)
{
	// Handle events in game (moves, bomb planting ...)
	switch ( reinterpret_cast< SDL_KeyboardEvent * >(event)->keysym.sym )
	{
	case SDLK_ESCAPE:
		changeMenu(Menu::MAIN);
		break;
	
	// Player 1
	case SDLK_UP:		// Up
		Player::getPlayer(1)->go(map::UP);
		break;
	case SDLK_DOWN:		// Down
		Player::getPlayer(1)->go(map::DOWN);
		break;
	case SDLK_RIGHT:	// Right
		Player::getPlayer(1)->go(map::RIGHT);
		break;
	case SDLK_LEFT:		// Left
		Player::getPlayer(1)->go(map::LEFT);
		break;
	case SDLK_RSHIFT:	// Bomb
		Player::getPlayer(1)->plantBomb();
		break;
	
	// Player 2
	case SDLK_e:		// Up
		Player::getPlayer(2)->go(map::UP);
		break;
	case SDLK_d:		// Down
		Player::getPlayer(2)->go(map::DOWN);
		break;
	case SDLK_f:		// Right
		Player::getPlayer(2)->go(map::RIGHT);
		break;
	case SDLK_s:		// Left
		Player::getPlayer(2)->go(map::LEFT);
		break;
	case SDLK_SPACE:	// Bomb
		Player::getPlayer(2)->plantBomb();
		break;
	
	default:
		break;
	}
	return 0;
}

void
Game::quit()
{
	if ( ! isInit )
		return;
	if ( playing )
	{
		playing = false;
		return;
	}
	
	// Clean everything before quitting
	Display::quit();
	map::Map::deleteMap();
	Game::waitThreads();
	bomb::Bomb::deInit();
	
	currentMenu = NULL;
	Menu::clear();
	
	// Write the new config file
	Config::write();
}

void
Game::waitThreads()
{
	// Wait for all threads launched by Game to exit
	for ( std::vector< SDL_Thread * >::iterator i = Game::threads.begin(),
		e = Game::threads.end() ; i != e ; ++i )
			SDL_WaitThread((*i), NULL);
	threads.clear();
}

