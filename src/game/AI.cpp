/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Hugo Mougard 2011 <mogzor@gmail.com>
 * Copyright (C) Kevin Decherf 2010 <kdecherf@gmail.com>
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

#include "AI.hpp"

using namespace bombherman;

void
AI::newAI()
{
	if (Player::players.size() >= static_cast<unsigned>(Config::getInt("maxPlayers")))
		throw exceptions::TooManyPlayersException();
	Player::players.push_back(new AI());
}

void
AI::init()
{
	if ( SDL_CreateThread(run, this) == NULL )
		bherr <<  "Unable to create thread to manage an AI player : " << SDL_GetError();
}

int
AI::run(void * param)
{
	AI * ai = static_cast<AI *>(param);
	
	// Keep alive the thread during entire game
	while (true)
	{
		while(ai->isAlive())
		{
			// Prepare map for AI
			ai->findPath();
		}
		SDL_Delay(300);
	}

}

void
AI::findPath()
{
	int maxRange = Config::getInt("maxRange");
	int maxItems = maxRange*maxRange*4;
	
	// Max points with setting maxRange
	int maxLeft = this->coords.x - maxRange;
	int maxRight = this->coords.x + maxRange;
	int maxTop = this->coords.y + maxRange;
	int maxBottom = this->coords.y - maxRange;
	
	int _depth = 1;
	// Prepare the array checker
	map::Coords array[maxItems];
	
	map::Coords current = this->coords;
	
	while (current.x > maxLeft && current.x < maxRight && current.y > maxTop && current.y < maxBottom)
	{
		// A* algorithm
		
		// Left point
		
	}
}

