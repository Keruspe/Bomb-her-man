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
	
	map::Coords _current;
	map::Direction _direction;

	// Keep alive the thread during entire game
	while (true)
	{
		while(ai->isAlive())
		{
			map::Coords current = ai->getCoords();
			map::Direction direction = ai->getOrient();
			
			int maxRange = Config::getInt("maxRange");
			int maxItems = (_maxRange*2)*(_maxRange*2);
			
			// Prepare map for AI
			findPath(maxRange,current);
		}
		SDL_Delay(300);
	}

}

void
AI::findPath(int _maxRange, map::Coords _ppoint)
{
	int _maxItems = (_maxRange*2)*(_maxRange*2);

	// Max points with setting maxRange
	int _maxLeft = _current.x - _maxRange;
	int _maxRight = _current.x + _maxRange;
	int _maxTop = _current.y + _maxRange;
	int _maxBottom = _current.y - _maxRange;

	Coords _current = _ppoint;
	int _depth = 1;
	// Prepare the array checker
	Coords[_maxItems] _array;
	
	while ((_current.x > _maxLeft && _current.x < _maxRight) && (_current.y > _maxTop && _current.y < _maxBottom))
	{
		// A* algorithm

		// Left point
		
	}
}
