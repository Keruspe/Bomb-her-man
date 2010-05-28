/* 
 * File:   Bomb.cpp
 * Author: mogzor
 * 
 * Created on May 25, 2010, 8:15 PM
 */

#include <iostream>

#include "bomb.hpp"
#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

SDL_mutex * Bomb::mutex = SDL_CreateMutex ();

Bomb::Bomb (Player * player) : player (player),
		coords (player->getCoords ()),
		exploded (false)
{
	Display::plantBomb (coords);
	SDL_Thread *thread;
	if ((thread = SDL_CreateThread(wait, this)) == NULL)
		bherr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
}

Player *
Bomb::getPlayer ()
{
	return this->player;
}

map::Coords *
Bomb::getCoords ()
{
	return & this->coords;
}

Bomb::~Bomb ()
{
}

int
Bomb::wait (void * param)
{
	SDL_Delay (5000);
	static_cast<Bomb * >(param)->explode();
	return 0;
}

void
Bomb::explode()
{
	if ( this->exploded ) return;
	SDL_LockMutex (mutex);
	this->exploded = true;
	std::vector<map::Coords> explodedCells;
	int range = static_cast<Uint32>(this->player->getRange ());
	if (coords.x != 0)
	{
		for(int x = coords.x, xFixed = coords.x; x >= (xFixed - range) && (x >= 0); -- x)
		{
			if (! check(x, coords.y))
				break;
			explodedCells.push_back (map::Coords(x, coords.y));
		}
	}
	if (coords.x != coords.max)
	{
		for(int x = coords.x + 1, xFixed = coords.x; x <= (xFixed + range) && (x <= coords.max); ++ x)
		{
			if (! check(x, coords.y))
				break;
			explodedCells.push_back (map::Coords(x, coords.y));
		}
	}
	if (coords.y != 0)
	{
		for(int y = coords.y - 1, yFixed = coords.y; y >= (yFixed - range) && (y >= 0); -- y)
		{
			if (! check(coords.x, y))
				break;
			explodedCells.push_back (map::Coords(coords.x, y));
		}
	}
	if (coords.y != coords.max)
	{
		for(int y = coords.y + 1, yFixed = coords.y; y <= (yFixed + range) && (y <= coords.max); ++ y)
		{
			if (! check(coords.x, y))
				break;
			explodedCells.push_back (map::Coords(coords.x, y));
		}
	}
	map::Map::removeBomb (coords);
	Display::explode (coords, explodedCells);
	SDL_UnlockMutex (mutex);
}

bool
Bomb::check (int x, int y)
{
	map::Coords coords(x, y);
	char item = map::Map::get(coords);
	switch ( item )
	{
		case map::INDESTRUCTIBLE :
			return false;
		case map::BARREL :
			map::Map::destroy (coords);
		break;
		case map::PLAYER :
			this->player->kill(Player::playerAt(coords));
		break;
		case map::PLAYONBOMB :
			this->player->kill(Player::playerAt(coords));
		case map::BOMB :
			AtomicCenter::getBomb(x, y)->explode();
		break;
		default : // Bonuses
			map::Map::removeBonus (coords);
		break;
	}
	return true;
}
