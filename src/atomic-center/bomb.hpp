/* 
 * File:   Bomb.hpp
 * Author: mogzor
 *
 * Created on May 25, 2010, 8:15 PM
 */

#ifndef _BOMB_HPP
#define	_BOMB_HPP

#include "map/map-utils.hpp"
#include "game/player.hpp"
#include "map/map.hpp"
#include "bombherman.hpp"
#include <SDL.h>

#include <vector>

namespace bombherman
{
	namespace bomb
	{
		class Bomb
		{
		public:
			Bomb (int, map::Coords);
			map::Coords & getCoords ();
			static void doExplode(Bomb *);
			static int waitExplode(void *);
			virtual ~Bomb ();
			
			static void deinit() { SDL_DestroyMutex(mutex); }
			
		private:
			void explode();
			bool check(Uint32, Uint32);
			void chainBomb(Bomb *);
			
			static SDL_mutex * mutex;
			SDL_sem *explosion;
			SDL_mutex *refsMutex;
			
			int player;
			map::Coords coords;
			std::vector<map::Coords> explodedCells;
			std::vector<Bomb *> chain;
			Uint8 refs;
		};
	}
}

#endif	/* _BOMB_HPP */

