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

Bomb::Bomb (int player, map::Coords coords) :
	explosion(SDL_CreateSemaphore(0)),
	refsMutex(SDL_CreateMutex()),
	player (player),
	coords (coords),
	refs(1)
{
	Display::plantBomb (coords);
	
	if ( SDL_CreateThread(waitExplode, this) == NULL )
		bherr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
}

Bomb::~Bomb()
{
	bhout << "Sorry, bomb " << std::ios::hex << this << " is died" << bhendl;
	SDL_DestroyMutex(refsMutex);
	SDL_DestroySemaphore(explosion);
}

int
Bomb::waitExplode(void *p)
{
	Bomb *b = static_cast<Bomb * >(p);
	SDL_LockMutex(b->refsMutex);
	SDL_SemWaitTimeout(b->explosion, 5000);
	bhout << "Bomb " << std::ios::hex << b <<  " will explode now !" << bhendl;
	b->explode();
	SDL_UnlockMutex(b->refsMutex);
	return 0;
}

void
Bomb::doExplode(Bomb *b)
{
	bhout << "Bomb " << std::ios::hex << b <<  " will explode" << bhendl;
	SDL_SemPost(b->explosion);
	bhout << "Bomb " << std::ios::hex << b <<  " is now able to explode" << bhendl;
	SDL_LockMutex(b->refsMutex);
	if ( --b->refs < 1 )
		delete(b);
	else
		SDL_UnlockMutex(b->refsMutex);
}

void
Bomb::explode()
{
	SDL_LockMutex(mutex);
	bhout << "Explosing bomb " << std::ios::hex << this << bhendl;
	Player * p = Player::getPlayer(this->player);
	if ( ! p )
	{
		SDL_UnlockMutex(mutex);
		return;
	}
	Uint32 range = static_cast<Uint32>(p->getRange());
	if ( map::Map::get(coords) == map::PLAYONBOMB )
	{
		p->kill(Player::playerAt(coords));
		explodedCells.push_back(coords);
	}
	bool up(true), down(true), left(true), right(true);
	for ( Uint32 i = 1 ; i <= range ; ++i )
	{
		if ( up )	up =	check(coords.x, coords.y - i);
		if ( down )	down =	check(coords.x, coords.y + i);
		if ( right )	right =	check(coords.x - i, coords.y);
		if ( left )	left =	check(coords.x + i, coords.y);
	}
	p->bombHasExploded();
	AtomicCenter::removeBomb(coords);
	map::Map::removeBomb(coords);
	Display::explode(coords, explodedCells);
	explodedCells.clear();
	SDL_UnlockMutex(mutex);
	for ( std::vector<Bomb *>::iterator i = chain.begin(), e = chain.end() ; i != e ; ++i )
		doExplode(*i);
	chain.clear();
}

bool
Bomb::check (Uint32 x, Uint32 y)
{
	map::Coords c(x, y);
	if ( ! c.validate() ) return false;
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
			chainBomb(AtomicCenter::getBomb(c));
		break;
		case map::NOTHING :
		break;
		default : // Bonuses
			map::Map::removeBonus(c);
	}
	explodedCells.push_back(c);
	return true;
}

void
Bomb::chainBomb(Bomb *b)
{
	++b->refs;
	chain.push_back(b);
}
