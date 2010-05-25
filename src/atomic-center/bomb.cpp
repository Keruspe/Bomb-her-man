/* 
 * File:   Bomb.cpp
 * Author: mogzor
 * 
 * Created on May 25, 2010, 8:15 PM
 */

#include "bomb.hpp"

Bomb::Bomb (Player * player, Coords * coords) : player (player),
		coords (coords),
		exploding (false)
{
	SDL_Thread *thread;
	if ((thread = SDL_CreateThread(AtomicCenter::wait, this)) == NULL)
		cerr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
	cout << "!!! Attention la bombe va exploser dans 5 secondes ..." << endl;
	this->threadId = SDL_GetThreadID (thread);
}

Player *
Bomb::getPlayer ()
{
	return this->player;
}

Uint32
Bomb::getThreadId ()
{
	return this->threadId;
}

Coords *
Bomb::getCoords ()
{
	return this->coords;
}

bool
Bomb::isExploding ()
{
	return this->exploding;
}

Bomb::~Bomb ()
{
}
