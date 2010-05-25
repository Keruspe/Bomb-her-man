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
#include "atomic-center.hpp"
#include <SDL/SDL.h>

using namespace bombherman;
using namespace bombherman::map;
using namespace bombherman::bomb;
using namespace std;

class Bomb {
public:
    Bomb (Player * player, Coords * coords);
    Player * getPlayer ();
    Uint32 getThreadId ();
    Coords * getCoords ();
    bool isExploding ();
    virtual ~Bomb ();
private:
    Player * player;
    Uint32 threadId;
    Coords * coords;
    bool exploding;
};

#endif	/* _BOMB_HPP */

