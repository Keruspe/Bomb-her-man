/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 * Copyright (C) Hugo Mougard 2010 <mogzor@gmail.com>
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
			 * @param _range The range of the explosion (Uint32)
			 */
			Bomb(int player, map::Coords c, Uint32 _range);
			
			/// Copy constructor
			/**
			 * @param other The bomb to copy
			 */
			Bomb(const Bomb & other);
			
			/// Operator=
			/**
			 * @return other The bomb to copy
			 */
			Bomb & operator=(const Bomb & other);
			
			/// Make a bomb explode
			/**
			 * @param b A pointer to the bomb which will explode (Bomb *)
			 */
			static void doExplode(Bomb * b);
			
			/// Free the mutex
			static void deInit() { SDL_DestroyMutex(mutex); }

			/// Start a new game
			static void newGame() { Bomb::gameOver = false; }
			
			/// Range getter
			/**
			 * @return The range of the explosion (Uint32)
			 */
			Uint32 getRange() { return this->range; }
			
			/// Destructor
			virtual ~Bomb() { SDL_DestroySemaphore(explosion); }
		private:
			static int waitExplode(void *);
			void explode();
			bool check(Uint32, Uint32);
			static SDL_mutex * mutex;
			SDL_sem * explosion;
			int player;
			Uint32 range;
			map::Coords coords;
			std::vector<map::Coords> explodedCells;
			std::vector<map::Coords> chain;
			static bool gameOver;
		};
	}
}

#endif	/* _BOMB_HPP */

