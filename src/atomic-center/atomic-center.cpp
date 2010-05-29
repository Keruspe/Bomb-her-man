/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

std::vector<std::vector<Bomb *> > AtomicCenter::bombs;

void
AtomicCenter::boum ()
{
	for ( std::vector<std::vector<Bomb *> >::iterator i = bombs.begin (), iEnd = bombs.end(); i != iEnd; ++i )
	{
		for ( std::vector<Bomb *>::iterator j = i->begin (), jEnd = i->end (); j != jEnd; ++j )
		{
			if ( *j )
				Bomb::doExplode(*j);
		}
	}
	SDL_Delay(250);
}

void
AtomicCenter::plantBomb (int player, map::Coords coords)
{
	if ( bombs.empty() )
	{
		int size = Config::getInt("mapSize");
		bombs.resize(size);
		for (int i = 0; i < size; ++i)
			bombs[i].resize(size);
	}
	if ( map::Map::plantBomb(coords) )
	{
		Bomb * bomb = new Bomb(player, coords);
		bombs[coords.x][coords.y] = bomb;
	}
}

void
AtomicCenter::removeBomb (map::Coords c)
{
	bombs[c.x][c.y] = NULL;
}

Bomb *
AtomicCenter::getBomb(map::Coords c)
{
	return bombs[c.x][c.y];
}
