/* 
 * File:   Bomb.hpp
 * Author: mogzor
 *
 * Created on May 25, 2010, 8:15 PM
 */

#ifndef _BOMB_HPP
#define _BOMB_HPP

#include <SDL.h>
#include <vector>

#include "bombherman.hpp"
#include "map/map-utils.hpp"
#include "game/player.hpp"
#include "map/map.hpp"

namespace bombherman
{
	namespace bomb
	{
		/// A bomb
		class Bomb
		{
		public:
			/// Constructor
			/**
			 * @param player The id of the player who planted the bomb (int)
			 * @param c The coords where the bomb has been planted (map::Coords)
			 */
			Bomb (int player, map::Coords c);
			
			/// Make a bomb explode
			/**
			 * @param b A pointer to the bomb which will explode (Bomb *)
			 */
			static void doExplode(Bomb * b);
			
			/// Free the mutex
			static void deInit() { SDL_DestroyMutex(mutex); }

			/// Start a new game
			static void newGame() { Bomb::gameOver = false; }
		private:
			/// Destructor
			virtual ~Bomb ();
			static int waitExplode(void *);
			void explode();
			bool check(Uint32, Uint32);
			static SDL_mutex * mutex;
			SDL_sem *explosion;
			int player;
			map::Coords coords;
			std::vector<map::Coords> explodedCells;
			std::vector<map::Coords> chain;
			static bool gameOver;
		};
	}
}

#endif	/* _BOMB_HPP */

