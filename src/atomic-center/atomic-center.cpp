/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
 * Copyright (C) Hugo Mougard 2010 <mogzor@gmail.com>
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
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

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

// Initialize statics
std::vector<std::vector<Bomb *> > AtomicCenter::bombs;
int AtomicCenter::numberOfBombs = 0;

void
AtomicCenter::boum ()
{
	// Make all bombs explode
	for ( std::vector<std::vector<Bomb *> >::iterator i = bombs.begin(),
		iEnd = bombs.end() ; i != iEnd ; ++i )
			for ( std::vector<Bomb *>::iterator j = i->begin(),
				jEnd = i->end(); j != jEnd; ++j )
					if ( *j )
						Bomb::doExplode(*j);
	
	// We wait until all bombs have exploded (custom "waitThreads")
	while (AtomicCenter::numberOfBombs > 0)
		SDL_Delay(250);
	Bomb::newGame();
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
	
	// The bomb has been planted
	Bomb * bomb = new Bomb(player, c);
	bombs[c.x][c.y] = bomb;
	++AtomicCenter::numberOfBombs;
	return true;
}

