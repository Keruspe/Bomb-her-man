/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

#include <SDL_thread.h>

#include "player.hpp"
#include "game.hpp"
#include "exceptions/too-many-players-exception.hpp"
#include "atomic-center/atomic-center.hpp"

using namespace bombherman;

// Initialize statics
std::vector<Player * > Player::players;
unsigned Player::icyDeadPeople = 0;

Player::Player() : plantableBombs (Config::getInt("defaultPlantableBombs")),
		range (Config::getInt("defaultRange")),
		plantedBombs (0),
		score (0),
		id (Player::players.size() + 1),
		coords (map::Coords()),
		orient(map::DOWN),
		alive(true)
{
}

Player *
Player::getPlayer(int id)
{
	if (Player::players.size() < static_cast<unsigned>(id) || 0 >= id)
		return 0;
	return Player::players[id - 1];
}

void
Player::newPlayer()
{
	if (Player::players.size() >= static_cast<unsigned>(Config::getInt("maxPlayers")))
		throw exceptions::TooManyPlayersException();
	Player::players.push_back(new Player());
}

void
Player::clean()
{
	for (std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end(); i != i_end ; ++i)
	{
		delete(*i);
	}
	Player::players.clear();
}

bool
Player::kill(Player * killed)
{
	if (! killed->alive)
		return false;
	if ( killed == this )
		this->addToScore(Config::getInt("suicideMalus"));
	else
		this->addToScore(Config::getInt("killBonus"));
	return killed->die();
}

bool
Player::die()
{
	this->alive = false;
	if ( ( Player::players.size() - ++Player::icyDeadPeople ) > 1 )
	{
		map::Map::removePlayer(this->coords);
		return false;
	}
	else if ( ( Player::players.size() - Player::icyDeadPeople ) == 1 )
		// Don't reinit twice or more in a game, or weird things will happen :)
		SDL_CreateThread(Player::reInit, NULL);
	return true;
}

int
Player::reInit(void * dummy)
{
	// Make everything explode
	bomb::AtomicCenter::boum();
	
	// Noone's dead anymore
	Player::icyDeadPeople = 0;
	
	// Reset players' defaults
	for (std::vector< Player * > ::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i)
			(*i)->resetToDefaultStats();
	
	// Go to next map
	Game::nextMap();
	return 0;
}

void
Player::setRange(int range)
{
	if (range < Config::getInt("minRange"))
		this->range = Config::getInt("minRange");
	else if (range > Config::getInt("maxRange"))
		this->range = Config::getInt("maxRange");
	else
		this->range = range;
}

map::MoveResult
Player::go(map::Direction & direction)
{
	if (! this || ! this->alive)
		// If we don't exist (new Game) or we're die, nothing'll happen
		return map::NOTHINGHAPPENED;
	bool orientChanged = (this->orient != direction);
	if (orientChanged)
		this->orient = direction;
	map::MoveResult moveResult = map::Map::movePlayer(this->coords, direction);
	if (moveResult == map::NOTHINGHAPPENED && orientChanged)
		// Only orient has changed
		return map::ORIENTCHANGED;
	return moveResult;
}

void
Player::addToScore(int score)
{
	if ( (this->score += score) < Config::getInt("minimumScore") )
		this->score = -9;
	else if (this->score > Config::getInt("maximumScore"))
		this->score = 99;
}

void
Player::setPlantableBombs(int plantableBombs)
{
	if(plantableBombs < Config::getInt("minCapacity"))
		this->plantableBombs = Config::getInt("minCapacity");
	else if (plantableBombs > Config::getInt("maxCapacity"))
		this->plantableBombs = Config::getInt("maxCapacity");
	else
		this->plantableBombs = plantableBombs;
}

void
Player::resetToDefaultStats()
{
	this->plantableBombs = Config::getInt("defaultPlantableBombs");
	this->range = Config::getInt("defaultRange");
	this->plantedBombs = 0;
	this->alive = true;
}

void
Player::plantBomb()
{
	if (! this->isAbleToPlantBomb())
		return;
	if ( bomb::AtomicCenter::plantBomb(this->id, this->coords) )
		// The bomb has been planted !
		++this->plantedBombs;
}

Player *
Player::playerAt(map::Coords & c)
{
	// We read all players
	for (std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i)
			// Coords are equal ? That's the one we're looking for !
			if((*i)->getCoords().x == c.x && (*i)->getCoords().y == c.y)
				return *i;
	return 0;
}

