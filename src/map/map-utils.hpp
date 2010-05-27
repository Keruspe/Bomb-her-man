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
#include <SDL_types.h>

#include "config/config.hpp"

namespace bombherman
{
	namespace map
	{
		typedef struct Grid
		{
			Uint32 size;
			std::vector< std::vector< char > > grid;
			bool exists;
			
			Grid() : size(static_cast<Uint32>(Config::getInt("mapSize"))),
				grid(std::vector< std::vector< char > >()),
				exists(false)
			{
			}
			
			std::vector< char > & operator[] (Uint32 y)
			{
				return grid[y];
			}
		} Grid;
		
		typedef struct Coords
		{
			Uint32 x;
			Uint32 y;
			Uint32 max;
			
			Coords() : x(-1), y(-1),
				max(static_cast<Uint32>(Config::getInt("mapSize") - 1))
			{
			}
		} Coords;
		
		typedef enum
		{
			UP = 0,
			DOWN = 1,
			LEFT = 2,
			RIGHT = 3
		} Direction;
		
		typedef enum
		{
			BOMB = 'b',
			BARREL = 't',
			INDESTRUCTIBLE = 'x',
			NOTHING = ' ',
			PLAYER = 'p',
			PLAYONBOMB = 'z'
		} Element;
		
		typedef enum
		{
			NONE = NOTHING,
			BOMBUP = '1',
			BOMBDOWN = '2',
			FIREUP = '3',
			FIREDOWN = '4',
			FULLFIRE = '5',
			NULLFIRE = '6'
		} Bonus;
	}
}

#endif // _MAP_UTILS_HPP_
 
