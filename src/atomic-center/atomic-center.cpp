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
int AtomicCenter::numberOfBombs = 0;

void
AtomicCenter::boum ()
{
	for ( std::vector<std::vector<Bomb *> >::iterator i = bombs.begin(),
		iEnd = bombs.end() ; i != iEnd ; ++i )
			for ( std::vector<Bomb *>::iterator j = i->begin(),
				jEnd = i->end(); j != jEnd; ++j )
					if ( *j )
						Bomb::doExplode(*j);
	while (AtomicCenter::numberOfBombs > 0)
		SDL_Delay(250);
}

bool
AtomicCenter::plantBomb (int player, map::Coords & c)
{
	if ( bombs.empty() )
	{
		int size = Config::getInt("mapSize");
		bombs.resize(size);
		for (int i(0) ; i < size ; ++i)
			bombs[i].resize(size);
	}
	if ( ! map::Map::plantBomb(c) )
		return false;
	Bomb * bomb = new Bomb(player, c);
	bombs[c.x][c.y] = bomb;
	++AtomicCenter::numberOfBombs;
	return true;
}

void
AtomicCenter::removeBomb (map::Coords & c)
{
	bombs[c.x][c.y] = NULL;
}

Bomb *
AtomicCenter::getBomb(map::Coords & c)
{
	return bombs[c.x][c.y];
}

