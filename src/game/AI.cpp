/* 
 * File:   AI.cpp
 * Author: mogzor
 * 
 * Created on May 29, 2010, 8:44 PM
 */

#include <SDL/SDL_timer.h>

#include "AI.hpp"

AI::AI () : Player ()
{
	init();
}

void
AI::newPlayer ()
{
	if (Player::players.size() >= static_cast<unsigned>(Config::getInt("maxPlayers")))
		throw exceptions::TooManyPlayersException();
	Player::players.push_back(new AI());
}

void
AI::init ()
{
	if ( SDL_CreateThread(doThings, this) == NULL )
		bherr <<  "Unable to create thread to manage an AI player : " << SDL_GetError();
}

int
AI::doThings (void * param)
{
	AI * ai = static_cast<AI *>(param);
}