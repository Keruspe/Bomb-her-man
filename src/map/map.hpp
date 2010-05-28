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
			static void newMap();
			static void newMap(std::string);
			static void deleteMap();
			static bool isPlayer(char);
			static bool plantBomb(Coords &);
			static char get(Coords);
			static char get(Uint32, Uint32);
			static MoveResult movePlayer(Coords &, Direction &);
			static void destroy(Coords &);
			static void toString();
			static void removePlayer(Coords &);
			static void removeBomb(Coords &);
			static void removeBonus(Coords &);
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

