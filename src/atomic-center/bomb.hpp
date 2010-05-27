/* 
 * File:   Bomb.hpp
 * Author: mogzor
 *
 * Created on May 25, 2010, 8:15 PM
 */

#ifndef _BOMB_HPP
#define	_BOMB_HPP

#include <iostream>
#include "map/map-utils.hpp"
#include "game/player.hpp"
#include <SDL/SDL.h>


namespace bombherman
{
    namespace bomb
    {
        class Bomb {
        public:
            Bomb (Player * player, map::Coords coords);
            Player * getPlayer ();
            Uint32 getThreadId ();
            map::Coords getCoords ();
            bool isExploded ();
            virtual ~Bomb ();
        private:
            static void explode (Bomb *);
            static int wait (void *);
            Player * player;
            Uint32 threadId;
            map::Coords coords;
            bool exploded;
        };
    }
}

#endif	/* _BOMB_HPP */

