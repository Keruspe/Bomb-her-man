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
		bhout << "We wants to boum bomb: " << std::ios::hex << (*i) << bhendl;
		Bomb::doExplode(*i);
	}
	bombs.clear();
	SDL_Delay(5000);
}

void
AtomicCenter::plantBomb (int player, map::Coords coords)
{
	if (map::Map::plantBomb (coords))
	{
		Bomb * bomb = new Bomb(player, coords);
		bombs.push_back(bomb);
		bombsXY[coords.x][coords.y] = bomb;
		bhout << "We wants to create x=" << coords.x << " y=" << coords.y << " bomb: " << std::ios::hex << bombsXY[coords.x][coords.y] << bhendl;
	}
}

void
AtomicCenter::removeBomb (map::Coords coords)
{
	bhout << "We wants to remove x=" << coords.x << " y=" << coords.y << " bomb: " << std::ios::hex << bombsXY[coords.x][coords.y] << bhendl;
	bombsXY[coords.x][coords.y] = NULL;
}

Bomb *
AtomicCenter::getBomb(map::Coords c)
{
	bhout << "We wants the x=" << c.x << " y=" << c.y << " bomb: " << std::ios::hex << bombsXY[c.x][c.y] << bhendl;
	return bombsXY[c.x][c.y];
}
