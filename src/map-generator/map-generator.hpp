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

#ifndef _MAP_GENERATOR_HPP_
#define _MAP_GENERATOR_HPP_

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "map/map-utils.hpp"
#include "config/config.hpp"

namespace bombherman 
{
	namespace map
	{
		/// A random map generator
		class MapGenerator
		{
		public:
			/// Generate a random map
			/**
			  * @param g The grid in which to put the map (map::Grid)
			  */
			static void generate(Grid & g);
			
			/// Generate random coords to place players
			/**
			  * @return Random coords (map::Coords)
			  */
			static Coords getRandomCoords();
			
			/// Generate a random number
			/**
			  * @param min The minimum (int)
			  * @param max The maximum (int)
			  *
			  * @return Random number (int)
			  */
			static int random(int min, int max);
			
			/// Throw a 100 faces dice to check random probabilities
			/**
			  * @param percentage Percentage of chance to get True as a result (int)
			  *
			  * @return 80% of chances to get true ig 80 was the parameter
			  */
			static bool throwDice(int percentage);
		private:
			static bool testCellFull(Grid &, Coords &);
			static bool testCellLimited(Grid &, Coords &);
			static int horizontalScan(Grid &, Coords &);
		};
	}
}

#endif // _MAP_GENERATOR_HPP_

