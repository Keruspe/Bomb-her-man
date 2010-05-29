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

Bomb::Bomb (int player, map::Coords coords) : player (player),
		coords (coords),
		exploded (false)
{
	Display::plantBomb (coords);
	if (SDL_CreateThread(wait, this) == NULL)
		bherr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
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
	this->exploded = true;
	AtomicCenter::removeBomb(coords);
	Player * p = NULL;
	if ( ! ( p = Player::getPlayer(this->player) ) ) return;
	SDL_LockMutex (mutex);
	std::vector<map::Coords> explodedCells;
	int range = static_cast<Uint32>(p->getRange());
	if (coords.x != 0)
	{
		for(int x = coords.x -1, xFixed = coords.x; x >= (xFixed - range) && (x >= 0); -- x)
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
	p->bombHasExploded();
	map::Map::removeBomb (coords);
	Display::explode (coords, explodedCells);
	SDL_UnlockMutex (mutex);
}

bool
Bomb::check (int x, int y)
{
	map::Coords c(x, y);
	char item = map::Map::get(c);
	switch ( item )
	{
		case map::INDESTRUCTIBLE :
			return false;
		case map::BARREL :
			map::Map::destroy (c);
		break;
		case map::PLAYER :
			Player::getPlayer(this->player)->kill(Player::playerAt(c));
		break;
		case map::PLAYONBOMB :
			Player::getPlayer(this->player)->kill(Player::playerAt(c));
		case map::BOMB :
			AtomicCenter::getBomb(x, y)->explode();
		break;
		default : // Bonuses
			map::Map::removeBonus (c);
		break;
	}
	return true;
}
