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
bool Bomb::gameOver = false;

Bomb::Bomb (int player, map::Coords c) :
	explosion(SDL_CreateSemaphore(0)),
	player(player),
	coords(c)
{
	Display::plantBomb(c);
	if ( SDL_CreateThread(waitExplode, this) == NULL )
		bherr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
}

Bomb::~Bomb ()
{
	bhout << "Sorry, bomb " << std::ios::hex << this << " is died" << bhendl;
	SDL_DestroySemaphore(explosion);
}

int
Bomb::waitExplode(void *p)
{
	Bomb *b = static_cast<Bomb * >(p);
	SDL_SemWaitTimeout(b->explosion, 5000);
	bhout << "Bomb " << std::ios::hex << b <<  " will explode now !" << bhendl;
	b->explode();
	delete(b);
	return 0;
}

void
Bomb::doExplode(Bomb *b)
{
	if ( ! b )
		return;
	bhout << "Bomb " << std::ios::hex << b <<  " will explode" << bhendl;
	SDL_SemPost(b->explosion);
}

void
Bomb::explode()
{
	SDL_LockMutex(mutex);
	AtomicCenter::removeBomb(coords);
	Player * p = NULL;
	if ( ! ( p = Player::getPlayer(this->player) ) )
		return;
	Uint32 range = static_cast<Uint32>(p->getRange());
	if ( map::Map::get(coords) == map::PLAYONBOMB )
		if ( p->kill(Player::playerAt(coords)) )
			return;
	bool up(true), down(true), right(true), left(true);
	for ( Uint32 i = 1 ; i <= range ; ++i )
	{
		if ( up )
			up =    check(coords.x, coords.y - i);
		if ( down )
			down =  check(coords.x, coords.y + i);
		if ( right )
			right = check(coords.x - i, coords.y);
		if ( left )
			left =  check(coords.x + i, coords.y);
		if ( ( ! up ) && ( ! down ) && ( ! right ) && ( ! left ) )
			break;
	}
	if ( Bomb::gameOver )
		return;
	p->bombHasExploded();
	map::Map::removeBomb(coords);
	Display::explode(coords, explodedCells);
	explodedCells.clear();
	SDL_UnlockMutex(mutex);
	for ( std::vector<map::Coords>::iterator i = chain.begin(), e = chain.end() ; i != e ; ++i )
		doExplode(AtomicCenter::getBomb(*i));
	chain.clear();
}

bool
Bomb::check(Uint32 x, Uint32 y)
{
	map::Coords c(x, y);
	if ( ( ! c.validate() ) || Bomb::gameOver )
		return false;
	char item = map::Map::get(c);
	switch ( item )
	{
		case map::INDESTRUCTIBLE :
			return false;
		case map::BARREL :
			map::Map::destroy(c);
		break;
		case map::PLAYER :
			if ( Player::getPlayer(this->player)->kill(Player::playerAt(c)) )
				Bomb::gameOver = true;
		break;
		case map::PLAYONBOMB :
			if ( Player::getPlayer(this->player)->kill(Player::playerAt(c)) )
				Bomb::gameOver = true;
		case map::BOMB :
			chain.push_back(c);
		break;
		case map::NOTHING :
			break;
		default : // Bonuses
			map::Map::removeBonus(c);
	}
	return true;
}

void
Bomb::newGame()
{
	SDL_UnlockMutex(mutex);
	Bomb::gameOver = false;
}

