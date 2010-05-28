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
	for (std::vector<std::vector<Bomb *> >::iterator i = bombs.begin (), iEnd = bombs.end (); i != iEnd; ++ i)
		for (std::vector<Bomb *>::iterator j = i->begin (), jEnd = i->end (); j != jEnd; ++ j)
			if (* j)
				if (! (* j)->isExploded ())
					(* j)->explode(* j);
}

void
AtomicCenter::plantBomb (Player * player)
{
	Bomb * bomb;
	bombs.resize(15);
	for (int i = 0; i < 15; i ++)
	{
		bombs[i].resize (15);
	}
	map::Coords coords = player->getCoords ();
	if (map::Map::plantBomb (coords))
	{
		bomb = new Bomb(player);
		bombs[coords.x][coords.y] = bomb;
	}
}

void
AtomicCenter::removeBomb (Bomb * & bomb)
{
	map::Coords coords = * bomb->getCoords ();
	bombs[coords.x][coords.y] = NULL;
	delete (bomb);
	bomb = NULL;
}

Bomb *
AtomicCenter::getBomb(int x, int y)
{
	return bombs[x][y];
}

std::vector<std::vector<Bomb *> >
AtomicCenter::getBombs ()
{
	return bombs;
}

