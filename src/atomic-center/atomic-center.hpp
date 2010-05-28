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

#include "map/map-utils.hpp"
#include "display/display.hpp"
#include "game/player.hpp"
#include "map/map.hpp"
#include "bomb.hpp"
#include <SDL.h>

namespace bombherman
{
    namespace bomb
    {
        class AtomicCenter
        {
        public:
            static void plantBomb (Player *);
            static void removeBomb(Bomb * &);
            static std::map<map::Coords *, Bomb *> getBombs ();
            static Bomb * getBomb (map::Coords *);
        private:
            static std::map<map::Coords *, Bomb *> bombs;
        };
    }
}

#endif	/* _ATOMICCENTER_HPP */
