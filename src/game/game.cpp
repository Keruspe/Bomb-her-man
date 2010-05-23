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

display::elements::Menu *Game::actualMenu = NULL;
std::vector< SDL_Thread * > *Game::threads = new std::vector< SDL_Thread * >();
bool Game::isInit = false;
bool Game::playing = true;

void Game::init()
{
	display::Display::init();
	actualMenu = display::elements::Menu::getMenu(display::elements::Menu::MAIN);
	isInit = true;
}

void
Game::main()
{
	if ( ! isInit ) init();
	
	display::Display::displayMenu(actualMenu->getContent(), actualMenu->getActual());
	
	SDL_Event event;
	while ( ( playing ) && ( SDL_WaitEvent(&event) ) )
	{
		switch ( event.type )
		{
			case SDL_QUIT:
				playing = false;
			break;
			case SDL_KEYDOWN:
				if ( actualMenu )
					threads->push_back(SDL_CreateThread(&bombherman::Game::eventMenu, &event.key));
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
Game::changeMenu(display::elements::Menu::Type type)
{
	if ( actualMenu )
	{
		delete(actualMenu);
		actualMenu = NULL;
	}
	actualMenu = display::elements::Menu::getMenu(type);
	display::Display::displayMenu(actualMenu->getContent(), actualMenu->getActual());
}

void
Game::play()
{
	if ( actualMenu )
	{
		delete(actualMenu);
		actualMenu = NULL;
	}
	
	display::Display::displayMap();
}

void
Game::quit()
{
	if ( ! isInit ) return;
	
	display::Display::quit();
	
	if ( actualMenu )
	{
		delete(actualMenu);
		actualMenu = NULL;
	}
	
	std::vector< SDL_Thread * >::iterator i = threads->begin(), e = threads->end();
	for ( ; i != e ; ++i )
	{
		SDL_WaitThread((*i), NULL);
		threads->erase(i);
	}
	delete(threads);
}

int
Game::eventMenu(void *event)
{
	switch ( reinterpret_cast<SDL_KeyboardEvent *>(event)->keysym.sym )
	{
		case SDLK_UP:
			bhout << "UP pressed" << bhendl;
			display::Display::displayMenu(actualMenu->getContent(), actualMenu->up());
		break;
		case SDLK_DOWN:
			bhout << "DOWN pressed" << bhendl;
			display::Display::displayMenu(actualMenu->getContent(), actualMenu->down());
		break;
		//case SDLK_ENTER:
		case SDLK_RETURN:
			bhout << "ENTER pressed" << bhendl;
			actualMenu->action();
		break;
		default:
		break;
	}
	std::vector< SDL_Thread * >::iterator i = threads->begin(), e = threads->end();
	Uint32 id = SDL_ThreadID();
	for ( ; i != e ; ++i )
	{
		if ( id == SDL_GetThreadID(*i) )
		{
			threads->erase(i);
			break;
		}
	}
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
	return 0;
}
