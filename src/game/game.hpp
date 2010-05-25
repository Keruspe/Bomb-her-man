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

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "bombherman.hpp"

#include <vector>

#include "display/display.hpp"

namespace bombherman
{
	class Game
	{
	public:
		/// Init
		/**
		 * It initializes all the part of the game (display, sound, events)
		 */
		static void init();
		/// Main loop
		/**
		 * It launches the main loop, creating display and other
		 */
		static void main();
		/// Change the menu
		/**
		 * @param type The new menu type
		 * @param stopGame Set it to true (default) to stop the current game (if any)
		 */
		static void changeMenu(display::elements::Menu::Type type, bool stopGame = true);
		/// Play
		/**
		 * @param newGame Set it to true (default) if you want to start a new game
		 */
		static void play(bool newGame = true);
		/// Stop
		static void stop() { playing = false; }
		/// Quit
		static void quit();
	
	private:
		static bool isInit, playing;
		
		static void threadClean(Uint32 id);
		
		static int eventMenu(void *);
		static int eventGame(void *);
		
		// To store the actual menu
		static display::elements::Menu *currentMenu;
		static map::Map *currentMap;
		
		static std::vector< SDL_Thread * > *threads;
	};
}

#endif // _GAME_HPP_
