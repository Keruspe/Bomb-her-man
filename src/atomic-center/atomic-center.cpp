/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

std::vector<Bomb *> AtomicCenter::bombs;
std::vector<std::vector<Bomb *> > AtomicCenter::bombsXY;

void
AtomicCenter::init()
{
	int size = Config::getInt("mapSize");
	bombsXY.resize(size);
	for (int i = 0; i < size; ++i)
		bombsXY[i].resize(size);
}

void
AtomicCenter::boum ()
{
	for (std::vector<Bomb *>::iterator i = bombs.begin (), iEnd = bombs.end (); i != iEnd; ++ i)
	{
		(*i)->explode();
		delete(*i);
	}
	bombs.clear();
	SDL_Delay(250);
}

void
AtomicCenter::plantBomb (int player, map::Coords coords)
{
	if (map::Map::plantBomb (coords))
	{
		Bomb * bomb = new Bomb(player, coords);
		bombs.push_back(bomb);
		bombsXY[coords.x][coords.y] = bomb;
	}
}

void
AtomicCenter::removeBomb (map::Coords coords)
{
	bombsXY[coords.x][coords.y] = NULL;
}

Bomb *
AtomicCenter::getBomb(Uint32 x, Uint32 y)
{
	return bombsXY[x][y];
}
