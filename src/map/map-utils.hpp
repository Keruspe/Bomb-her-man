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
		/// The grid used by the map
		typedef struct Grid
		{
			/// The size of the Grid (Uint32)
			Uint32 size;
			
			/// The actual grid itself (std::vector< std::vector< char > >)
			std::vector< std::vector< char > > grid;
			
			/// Does the gris exist ? (Have it been initialised ?)
			bool exists;
			
			/// Grid constructor
			Grid() : size(static_cast<Uint32>(Config::getInt("mapSize"))),
				grid(std::vector< std::vector< char > >()),
				exists(false)
			{
			}
			
			
			/// Operator[] to allow you using grid[a][b] instead of grid.grid[a][b]
			/**
			 * @param y The number of the line, the first one being 0 (Uint32)
			 *
			 * @return The line tou want (std::vector< char >)
			 */
			std::vector< char > & operator[] (Uint32 y)
			{
				return grid[y];
			}
		} Grid;
		
		/// The coords of an element
		typedef struct Coords
		{
			/// The abscisse of the cell (Uint32)
			Uint32 x;
			
			/// The ordinate of the cell (Uint32)
			Uint32 y;
			
			/// The max value of x or y (Uint32)
			Uint32 max;
			
			/// Coords default constructor (sets Coords to mapSize,mapSize)
			Coords() : x(Config::getInt("mapSize")),
				y(Config::getInt("mapSize")),
				max(static_cast<Uint32>(Config::getInt("mapSize") - 1))
			{
			}
			
			/// Coords constructor
			/**
			 * @param _x Abscisse (Uint32)
			 * @param _y Ordinate (Uint32)
			 */
			Coords(Uint32 _x, Uint32 _y) : x(_x), y(_y),
				max(static_cast<Uint32>(Config::getInt("mapSize") - 1))
			{
			}
			
			/// Checks if coords are valid
			/**
			 * @return True if the coords are valid
			 */
			bool
			validate()
			{
				return (this->x <= this->max && this->y <= this->max);
			}
		} Coords;
		
		/// Different directions
		typedef enum
		{
			UP = 0, /** < Up */
			DOWN = 1, /** < Down */
			LEFT = 2, /** < Left */
			RIGHT = 3 /** < Right */
		} Direction;
		
		/// Elements of the map
		typedef enum
		{
			BOMB = 'b', /** < Bomb */
			BARREL = 't', /** < Barrel */
			INDESTRUCTIBLE = 'x', /** < Indestructible cell */
			NOTHING = ' ', /** < Empty cell */
			PLAYER = 'p', /** < Player */
			PLAYONBOMB = 'z' /** < Player on a bomb */
		} Element;
		
		/// Different kind of bonuses
		typedef enum
		{
			NONE = NOTHING, /** < None (is equal to the Nothing element) */
			BOMBUP = '1', /** < BombUp (increase the number of bombs you can carry) */
			BOMBDOWN = '2', /** < BombDown (decrease the number of bombs you can carry) */
			FIREUP = '3', /** < FireUp (increase the range of your bombs) */
			FIREDOWN = '4', /** < FireDown (decrease the range of your bombs) */
			FULLFIRE = '5', /** < FullFire (set the range of your bombs to the maximum allowed) */
			NULLFIRE = '6', /** < NullFire (set the range of your bombs to the minimum allowed) */
			FIRSTBONUS = BOMBUP,
			LASTBONUS = NULLFIRE
		} Bonus;
		
		/// Summary of what happened in a move
		typedef enum
		{
			ORIENTCHANGED = 4, /** < OrientChanged (no move, only an orientation change) */
			MOVED = 5, /** < Moved (the player moved) */
			BONUSTAKEN = 6, /** < BonusTaken (the player moved and took a bonus) */
			NOTHINGHAPPENED = 7 /** < NothingHappened */
		} MoveResult;
	}
}

#endif // _MAP_UTILS_HPP_
 
