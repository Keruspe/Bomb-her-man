/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <vector>

#include "map-parser.hpp"
#include "exceptions/map/malformed-file-exception.hpp"
#include "game/player.hpp"

namespace bombherman
{
	namespace map
	{	
		class Map
		{
		public:
			/// Create a new randomly generated map
			static void newMap();
			
			/// Create a new map from a file
			/**
			  * @param path Path to the file (std::string)
			  */
			static void newMap(std::string path);
			
			/// Delete the map
			static void deleteMap();
			
			/// Plant a bomb on the map
			/**
			  * @param c The coords where to plant the bomb (map::Coords)
			  */
			static bool plantBomb(Coords & c);
			
			/// What is there in this cell ?
			/**
			  * @param c The coords where to look for (map::Coords)
			  *
			  * @return The element in the corresponding cell (char)
			  */
			static char get(Coords & c);
			
			/// What is there in this cell ?
			/**
			  * @param x The abscisse (Uint32)
			  * @param y The ordinate (Uint32)
			  *
			  * @return The element in the corresponding cell (char)
			  */
			static char get(Uint32 x, Uint32 y);
			
			/// Make a player move
			/**
			  * @param c The coords from where the move start (map::Coords)
			  * @param direction The direction of the move (map::Direction)
			  *
			  * @return The sumary of the move (map::MoveResult)
			  */
			static MoveResult movePlayer(Coords & c, Direction & direction);
			
			/// Destroy a barrel
			/**
			  * @param c The coords of the barrel (map::coords)
			  */
			static void destroy(Coords & c);
			
			/// Display the map
			static void toString();
			
			/// Remove a player from the map
			/**
			  * @param c The coords of the player (map::Coords)
			  */
			static void removePlayer(Coords & c);
			
			/// Remove a bomb from the map
			/**
			  * @param c The coords of the bomb (map::Coords)
			  */
			static void removeBomb(Coords & c);
			
			/// Remove a bonus from the map
			/**
			  * @param c The coords of the bonus (map::Coords)
			  */
			static void removeBonus(Coords & c);
			
			/// Does the map exist ?
			/**
			  * @return True if the map exists
			  */
			static bool exists();
		private:
			static void placePlayers();
			static bool moveUp(Coords &);
			static bool moveDown(Coords &);
			static bool moveLeft(Coords &);
			static bool moveRight(Coords &);
			static bool applyBonus(Coords &);
			static void cleanOldSpot(Coords &);
			static Grid map;
		};
	}
}

#endif // _MAP_HPP_

