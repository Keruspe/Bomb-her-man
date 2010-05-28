/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include <iostream>

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

std::map<map::Coords *, Bomb *> AtomicCenter::bombs;

void
AtomicCenter::plantBomb (Player * player)
{
	Bomb * bomb;
	if (map::Map::plantBomb (player->getCoords ()))
	{
		bomb = new Bomb(player);
		bombs[bomb->getCoords ()] = bomb;
	}
}

void
AtomicCenter::removeBomb (Bomb * & bomb)
{
	bombs.erase (bomb->getCoords ());
	delete (bomb);
	bomb = NULL;
}

Bomb *
AtomicCenter::getBomb(map::Coords * coords)
{
	return bombs[coords];
}

std::map<map::Coords *, Bomb *>
AtomicCenter::getBombs ()
{
	return bombs;
}

