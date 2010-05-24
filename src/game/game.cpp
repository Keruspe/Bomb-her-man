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

#include "game.hpp"
#include "display/elements/menu.hpp"

using namespace bombherman;

display::elements::Menu *Game::currentMenu = NULL;
map::Map *Game::currentMap = NULL;
std::vector< SDL_Thread * > *Game::threads = new std::vector< SDL_Thread * >();
bool Game::isInit = false;
bool Game::playing = true;

void Game::init()
{
	display::Display::init();
	changeMenu(display::elements::Menu::MAIN);
	
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
				{
					//threads->push_back(SDL_CreateThread(&bombherman::Game::eventMenu, &event.key));
					eventMenu(&event.key);
				}
				else
				{
					//threads->push_back(SDL_CreateThread(&bombherman::Game::eventGame, &event.key));
					eventGame(&event.key);
				}
			break;
			default:
			break;
		}
	}
	quit();
}

void
Game::changeMenu(display::elements::Menu::Type type)
{
	if ( currentMap )
	{
		delete(currentMap);
		currentMap = NULL;
	}
	if ( currentMenu )
	{
		delete(currentMenu);
		currentMenu = NULL;
	}
	currentMenu = display::elements::Menu::getMenu(type);
	display::Display::displayMenu(currentMenu->getContent(), currentMenu->getCurrent());
}

void
Game::play()
{
	if ( currentMenu )
	{
		delete(currentMenu);
		currentMenu = NULL;
	}
	
	if ( currentMap )
	{
		delete(currentMap);
		currentMap = NULL;
	}
	
	currentMap = new map::Map();
        currentMap->newPlayer(1);
        currentMap->newPlayer(2);
	
	map::Coords coords;
	int size = Config::getInt("mapSize");
	for(coords.y = 0 ; coords.y < size ; ++coords.y)
	{
		for(coords.x = 0 ; coords.x < size ; ++coords.x)
		{
			std::cout << currentMap->get(coords);
		}
		std::cout << std::endl;
	}
	
	display::Display::displayMap(currentMap);
}

void
Game::quit()
{
	if ( ! isInit ) return;
	
	display::Display::quit();
	
	if ( currentMenu )
	{
		delete(currentMenu);
		currentMenu = NULL;
	}
	
	for ( std::vector< SDL_Thread * >::iterator i = threads->begin(), e = threads->end() ; i != e ; ++i )
	{
		SDL_WaitThread((*i), NULL);
	}
	delete(threads);
	
	Config::write();
}

void
Game::threadClean(Uint32 id)
{
	for ( std::vector< SDL_Thread * >::iterator i = threads->begin(), e = threads->end() ; i != e ; ++i )
	{
		if ( id == SDL_GetThreadID(*i) )
		{
			(*i) = NULL;
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
		case SDLK_UP:
			bhout << "UP pressed" << bhendl;
			display::Display::displayMenu(currentMenu->getContent(), currentMenu->up());
		break;
		case SDLK_DOWN:
			bhout << "DOWN pressed" << bhendl;
			display::Display::displayMenu(currentMenu->getContent(), currentMenu->down());
		break;
		case SDLK_KP_ENTER:
		case SDLK_SPACE:
		case SDLK_RETURN:
			bhout << "Menu action" << bhendl;
			currentMenu->action();
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
			bhout << "UP pressed" << bhendl;
			changeMenu(display::elements::Menu::MAIN);
		break;
		default:
		break;
	}
	
	Game::threadClean(SDL_ThreadID());
	
	return 0;
}
