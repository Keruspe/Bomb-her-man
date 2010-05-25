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
        class MapGenerator
        {
        public:
            static void generate(Grid&);
	    static Coords & getRandomCoords();
            static bool throwDice(int);
            static int random(int, int);
        private:
            static bool testCellFull(Grid&, Coords);
            static bool testCellLimited(Grid&, Coords);
            static int horizontalScan(Grid&, Coords);
        };
    }
}

#endif // _MAP_GENERATOR_HPP_
