/* 
 * File:   Bomb.hpp
 * Author: mogzor
 *
 * Created on May 25, 2010, 8:15 PM
 */

#ifndef _BOMB_HPP
#define	_BOMB_HPP

#include <iostream>
#include "map/map-utils.hpp"
#include "game/player.hpp"
#include "map/map.hpp"
#include "bombherman.hpp"
#include <SDL.h>


namespace bombherman
{
	namespace bomb
	{
		class Bomb
		{
		public:
			Bomb (int, map::Coords);
			map::Coords & getCoords ();
			void explode ();
			virtual ~Bomb ();
			
			static void deinit() { SDL_DestroyMutex(mutex); }
			
		private:
			bool exploded;
			static SDL_mutex * mutex;
			static int wait (void *);
			bool check (int, int);
			int player;
			map::Coords coords;
		};
	}
}

#endif	/* _BOMB_HPP */

