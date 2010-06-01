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

#include <iostream>

#include "bomb.hpp"
#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

// Initialize statics
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

int
Bomb::waitExplode(void *p)
{
	// Delay the explosion
	Bomb *b = static_cast<Bomb * >(p);
	SDL_SemWaitTimeout(b->explosion, 5000);
	b->explode();
	AtomicCenter::bombExploded();
	delete(b);
	return 0;
}

void
Bomb::doExplode(Bomb *b)
{
	// Force explosion
	if ( b )
		SDL_SemPost(b->explosion);
}

void
Bomb::explode()
{
	// The actual real explosion
	SDL_LockMutex(mutex);
	AtomicCenter::removeBomb(coords);
	Player * p = Player::getPlayer(this->player);
	if ( p ) // The player still exists (new Game ...)
	{
		Uint32 range = static_cast<Uint32>(p->getRange());
		if ( map::Map::get(coords) == map::PLAYONBOMB && ! Bomb::gameOver )
			if ( p->kill(Player::playerAt(coords)) )
				Bomb::gameOver = true;
		bool up(true), down(true), right(true), left(true);
		for ( Uint32 i(1) ; i <= range ; ++i )
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
	}
	if ( Bomb::gameOver )
	{
		// Game over, nothing to do
		SDL_UnlockMutex(mutex);
		return;
	}
	
	// Tell the player that his bomb has exploded
	p->bombHasExploded();
	map::Map::removeBomb(coords);
	Display::explode(coords, explodedCells);
	
	// Reinitialize explodedCells
	explodedCells.clear();
	SDL_UnlockMutex(mutex);
	
	// Make the bombs which were hit explode
	for ( std::vector<map::Coords>::iterator i = chain.begin(),
		e = chain.end() ; i != e ; ++i )
			doExplode(AtomicCenter::getBomb(*i));
	chain.clear();
}

bool
Bomb::check(Uint32 x, Uint32 y)
{
	map::Coords c(x, y);
	if ( ( ! c.validate() ) || Bomb::gameOver )
		return false;
	
	// Get element on cell
	char item = map::Map::get(c);
	switch ( item )
	{
		case map::INDESTRUCTIBLE :
			return false;
		case map::BARREL :
			map::Map::destroyBarrel(c);
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
	
	// Add Coords to explodedCells (for Display)
	explodedCells.push_back(c);
	return true;
}

