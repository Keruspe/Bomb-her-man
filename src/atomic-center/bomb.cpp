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
	std::cout << "!!! Attention la bombe va exploser dans 5 secondes ..." << bhendl;
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
	std::cout << "deletion" << std::endl;
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
	bomb->exploded = true;
	std::vector<map::Coords> explodedCells;
	SDL_LockMutex (mutex);
	Player player = * bomb->player;
	map::Coords coords = bomb->coords;
	int range = static_cast<Uint32>(player.getRange ());
	//parcourons la case de la bombe et les cases à gauche de la bombe
	if (coords.x != 0)
		for(int x = coords.x, xFixed = coords.x; x >= (xFixed - range) && (x >= 0); -- x)
		{
			if (! check(x, coords.y))
				break;
			explodedCells.push_back (map::Coords(x, coords.y));
		}
	if (coords.x != coords.max)
		for(int x = coords.x + 1, xFixed = coords.x; x <= (xFixed + range) && (x <= coords.max); ++ x)
		{
			if (! check(x, coords.y))
				break;
			explodedCells.push_back (map::Coords(x, coords.y));
		}
	if (coords.y != 0)
		for(int y = coords.y - 1, yFixed = coords.y; y >= (yFixed - range) && (y >= 0); -- y)
		{
			if (! check(coords.x, y))
				break;
			explodedCells.push_back (map::Coords(coords.x, y));
		}
	if (coords.y != coords.max)
		for(int y = coords.y + 1, yFixed = coords.y; y <= (yFixed + range) && (y <= coords.max); ++ y)
		{
			if (! check(coords.x, y))
				break;
			explodedCells.push_back (map::Coords(coords.x, y));
		}
	Display::explode (coords, explodedCells);
	map::Map::toString();
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
	case map::BOMB :
		if (AtomicCenter::getBomb (& coords))
			explode (AtomicCenter::getBomb (& coords));
		map::Map::removeBomb (& coords);
		break;
	case map::BARREL :
		map::Map::destroy (coords);
		break;
	case map::PLAYER :
		Player::playerAt (& coords)->die();
		break;
	case map::PLAYONBOMB :
		Player::playerAt (& coords)->die();
		break;
	}
	std::cout << "Parcours de : [" << x << ", "  << y << "]" << std::endl;
	return true;
}