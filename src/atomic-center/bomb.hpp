/* 
 * File:   Bomb.hpp
 * Author: mogzor
 *
 * Created on May 25, 2010, 8:15 PM
 */

#ifndef _BOMB_HPP
#define	_BOMB_HPP

#include "bombherman.hpp"
#include "map/map-utils.hpp"
#include "game/player.hpp"
#include "map/map.hpp"

#include <SDL.h>


namespace bombherman
{
	namespace bomb
	{
		class Bomb
		{
		public:
			Bomb (int, map::Coords);
			static void doExplode(Bomb *b);
			
			static void deinit() { SDL_DestroyMutex(mutex); }
			
		private:
			static int waitExplode(void *);
			
			void explode();
			bool check(Uint32, Uint32);
			
			virtual ~Bomb ();
		
			static SDL_mutex * mutex;
			
			SDL_sem *explosion;
			
			int player;
			map::Coords coords;
			std::vector<map::Coords> explodedCells;
			std::vector<map::Coords> chain;
			bool gameOver;
		};
	}
}

#endif	/* _BOMB_HPP */

