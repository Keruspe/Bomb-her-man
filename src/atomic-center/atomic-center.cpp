/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;
using namespace bombherman::map;
using namespace std;

std::vector<Bomb *> AtomicCenter::bombs;

void
AtomicCenter::plantBomb (Player * player, Coords c)
{
	Bomb * bomb = new Bomb(player, c);
	bombs.push_back (bomb);
}

void
AtomicCenter::removeBomb (Bomb* bomb)
{
	
}

std::vector<Bomb *>
AtomicCenter::getBombs ()
{
	return bombs;
}

