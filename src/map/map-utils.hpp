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

#ifndef _MAP_UTILS_HPP_
#define _MAP_UTILS_HPP_

#include <vector>

#include "config/config.hpp"

namespace bombherman
{
	namespace map
	{
		typedef struct Grid
		{
			int size;
			std::vector< std::vector< char > > grid;

			Grid() : size(static_cast<unsigned>(Config::getInt("mapSize"))),
				grid(std::vector< std::vector< char > >())
			{
			}

			std::vector< char > & operator[] (int y)
			{
				return grid[y];
			}
		} Grid;

		typedef struct Coords
		{
			int x;
			int y;
			int max;

			Coords() : max(Config::getInt("mapSize") - 1)
			{
			}
		} Coords;

		typedef enum
		{
			UP,
			DOWN,
			LEFT,
			RIGHT
		} Direction;

		typedef enum
		{
			BOMB='b',
			BARREL='t',
			INDESTRUCTIBLE='x',
			NONE=' '
		} Element;

		typedef enum
		{
			BOMBUP,
			BOMBDOWN,
			FIREUP,
			FIREDOWN,
			FULLFIRE
		} Bonus;
	}
}

#endif // _MAP_UTILS_HPP_
 
