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

bool
Bomb::isExploded ()
{
	return this->exploded;
}

Bomb::~Bomb ()
{
}

int
Bomb::wait (void * param)
{
	Bomb * bomb = static_cast<Bomb * >(param);
	SDL_Delay (5000);
	if (! bomb->exploded) {
		explode (bomb);
	}
	return 0;
}

void
Bomb::explode (Bomb * bomb)
{
	SDL_LockMutex (mutex);
	bomb->exploded = true;
	std::vector<map::Coords> explodedCells;
	Player player = * bomb->player;
	map::Coords coords = bomb->coords;
	int range = static_cast<Uint32>(player.getRange ());
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
	map::Map::removeBomb (& coords);
	Display::explode (coords, explodedCells);
	SDL_UnlockMutex (mutex);
}

bool
Bomb::check (int x, int y)
{
	map::Coords coords(x, y);
	char item = map::Map::get(coords);
	switch (item)
	{
	case map::INDESTRUCTIBLE :
		return false;
	case map::BARREL :
		map::Map::destroy (coords);
		break;
	case map::PLAYER :
		Player::playerAt (coords)->die();
		break;
	case map::PLAYONBOMB :
		Player::playerAt (coords)->die();
	case map::BOMB :
		if (! AtomicCenter::getBomb (x, y)->exploded)
			explode (AtomicCenter::getBomb (x, y));
		break;
	case map::BOMBDOWN :
	case map::BOMBUP :
	case map::FIREDOWN :
	case map::FIREUP :
	case map::FULLFIRE :
	case map::NULLFIRE :
		map::Map::removeBonus (& coords);
		break;
	}
	return true;
}
