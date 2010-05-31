/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) mogzor 2010 <mogzor@gmail.com>
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

#ifndef _ATOMICCENTER_HPP
#define	_ATOMICCENTER_HPP

#include <ios>
#include <iostream>
#include <SDL.h>

#include "display/display.hpp"
#include "game/player.hpp"
#include "map/map.hpp"
#include "map/map-utils.hpp"
#include "bomb.hpp"

namespace bombherman
{
	namespace bomb
	{
		/// Class used to manage Bombs
		class AtomicCenter
		{
		public:
			/// Plant a bomb
			/**
			 * @param player The id of the player who planted the bomb (int)
			 * @param c The coords where the bomb has been planted (map::Coords)
			 *
			 * @return True if a bomb has been planted
			 */
			static bool plantBomb (int player, map::Coords & c);
			
			/// Remove a bomb
			/*
			 * @param c The coords of the bomb to remove (map::Coords)
			 */
			static void removeBomb (map::Coords & c) { bombs[c.x][c.y] = NULL; }
			
			/// Get a bomb by its coords
			/*
			 * @param c The coords where you're looking for a bomb (map::Coords)
			 *
			 * @return A pointer to the bomb (Bomb *)
			 */
			static Bomb * getBomb (map::Coords & c) { return bombs[c.x][c.y]; }
			
			/// Make everything explode
			static void boum();

			/// Decrease the number of bombs still alive
			static void bombExploded() { --AtomicCenter::numberOfBombs; }
		private:
			static std::vector<std::vector<Bomb *> > bombs;
			static int numberOfBombs;
		};
	}
}

#endif	/* _ATOMICCENTER_HPP */

