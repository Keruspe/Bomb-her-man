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

using namespace bombherman;

Menu *Game::currentMenu = NULL;
std::vector< SDL_Thread * > *Game::threads = new std::vector< SDL_Thread * >();
bool Game::isInit = false;
bool Game::playing = true;

Sint32 Game::mapCount = -1;

void Game::init()
{
	Display::init();
	changeMenu(Menu::MAIN);
	
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
	
	isInit = true;
}

void
Game::main()
{
	if ( ! isInit ) init();
	
	SDL_Event event;
	while ( ( playing ) && ( SDL_WaitEvent(&event) ) )
	{
		switch ( event.type )
		{
			case SDL_QUIT:
				playing = false;
			break;
			case SDL_KEYDOWN:
				if ( currentMenu )
					SDL_CreateThread(&bombherman::Game::eventMenu, &event.key);
				else
					threads->push_back(SDL_CreateThread(&bombherman::Game::eventGame, &event.key));
			break;
			default:
			break;
		}
	}
	quit();
}

void
Game::changeMenu(Menu::Type type, bool stopGame)
{
	if ( ( stopGame ) )
	{
		map::Map::deleteMap();
		Player::clean();
		Display::quitGame();
		bomb::AtomicCenter::boum();
	}
	currentMenu = Menu::getMenu(type);
	Display::displayMenu(currentMenu);
}

void
Game::newGame()
{
	for ( int i = 0, e = Config::getInt("nbPlayers") ; i < e ; ++i )
		Player::newPlayer();
	//for ( int i = 0, e = Config::getInt("nbAIs") ; i < e ; ++i )
	//	AI::newAI();
	
	mapCount = Config::getInt("nbMaps");
	nextMap();
}

void
Game::nextMap()
{
	Game::waitThreads();
	map::Map::deleteMap();
	if ( --mapCount < 0 )
		changeMenu(Menu::MAIN);
	else
	{
		map::Map::newMap();
		play();
	}
}

void
Game::play()
{
	currentMenu = NULL;
	Display::updateMap();
}

void
Game::threadClean(Uint32 id)
{
	for ( std::vector< SDL_Thread * >::iterator i = threads->begin(), e = threads->end() ; i != e ; ++i )
	{
		if ( id == SDL_GetThreadID(*i) )
		{
			threads->erase(i);
			break;
		}
	}
}

int
Game::eventMenu(void *event)
{
	switch ( reinterpret_cast<SDL_KeyboardEvent *>(event)->keysym.sym )
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
	
	Game::threadClean(SDL_ThreadID());
	
	return 0;
}

int
Game::eventGame(void *event)
{
	switch ( reinterpret_cast<SDL_KeyboardEvent *>(event)->keysym.sym )
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
	
	Game::threadClean(SDL_ThreadID());
	
	return 0;
}

void
Game::quit()
{
	if ( ! isInit ) return;
	if ( playing )
	{
		playing = false;
		return;
	}
	
	Display::quit();
	
	map::Map::deleteMap();
	
	Game::waitThreads();
	delete(threads);
	
	bomb::Bomb::deInit();
	
	currentMenu = NULL;
	Menu::clear();
	
	Config::write();
}

void
Game::waitThreads()
{
	for ( std::vector< SDL_Thread * >::iterator i = Game::threads->begin(),
		e = Game::threads->end() ; i != e ; ++i )
			SDL_WaitThread((*i), NULL);
	threads->clear();
}

