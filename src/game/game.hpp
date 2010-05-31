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

#include <vector>

#include "bombherman.hpp"
#include "display/display.hpp"

namespace bombherman
{
	/// Class used to manage the game
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
		static void changeMenu(Menu::Type type, bool stopGame = true);
		
		/// Launch a new game
		static void newGame();
		
		/// Got to next map
		static void nextMap();
		
		/// Play
		static void play();
		
		/// Stop
		static void stop() { playing = false; }
	private:
		static bool isInit, playing;
		
		/// Quit
		static void quit();
		
		#ifdef THREAD_EVENTS
		static std::vector< SDL_Thread * > *threads;
		static void threadClean(Uint32 id);
		#endif // THREAD_EVENTS
		
		static int eventMenu(void *);
		static int eventGame(void *);
		
		// To store the actual menu
		static Menu *currentMenu;
		
		static Sint32 mapCount;
	};
}

#endif // _GAME_HPP_

