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
#include "display/display.hpp"

using namespace bombherman;

// Initialize statics
std::vector< Player * > Player::players;
unsigned Player::icyDeadPeople = 0;

Player::Player() :
	plantableBombs(Config::getInt("defaultPlantableBombs")),
	range(Config::getInt("defaultRange")),
	plantedBombs(0),
	score(0),
	id(Player::players.size() + 1),
	alive(true),
	coords(map::Coords()),
	orient(map::DOWN),
	move_mutex(SDL_CreateMutex()),
	currentMoves(0)
{
}

Player::Player(const Player & other) :
	plantableBombs(other.plantableBombs),
	range(other.range),
	plantedBombs(other.plantedBombs),
	score(other.score),
	id(other.id),
	alive(other.alive),
	coords(other.coords),
	orient(other.orient),
	move_mutex(other.move_mutex),
	currentMoves(other.currentMoves)
{
}

Player &
Player::operator=(const Player & other)
{
	this->plantableBombs = other.plantableBombs;
	this->range = other.range;
	this->plantedBombs = other.plantedBombs;
	this->score = other.score;
	this->id = other.id;
	this->alive = other.alive;
	this->coords = other.coords;
	this->orient = other.orient;
	this->move_mutex = other.move_mutex;
	this->currentMoves = other.currentMoves;
	return *this;
}

Player *
Player::getPlayer(int id)
{
	if ( Player::players.size() < static_cast<unsigned>(id) || 0 >= id )
		return 0;
	return Player::players[id - 1];
}

void
Player::newPlayer()
{
	if ( Player::players.size() >= static_cast< unsigned >(Config::getInt("maxPlayers")) )
		throw exceptions::TooManyPlayersException();
	Player::players.push_back(new Player());
}

void
Player::clean()
{
	for ( std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end(); i != i_end ; ++i )
	{
		delete(*i);
	}
	Player::players.clear();
}

bool
Player::kill(Player * killed)
{
	if ( ! killed->alive )
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
Player::reInit(void *)
{
	// Make everything explode
	bomb::AtomicCenter::boum();
	
	// Noone's dead anymore
	Player::icyDeadPeople = 0;
	
	// Reset players' defaults
	for ( std::vector< Player * > ::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i )
			(*i)->resetToDefaultStats();
	
	// Go to next map
	Game::nextMap();
	return 0;
}

void
Player::setRange(Uint32 range)
{
	if (range < static_cast< Uint32 >(Config::getInt("minRange")))
		this->range = static_cast< Uint32 >(Config::getInt("minRange"));
	else if (range > static_cast< Uint32 >(Config::getInt("maxRange")))
		this->range = static_cast< Uint32 >(Config::getInt("maxRange"));
	else
		this->range = range;
}

void
Player::go(map::Direction direction)
{
	if ( ! this )
		// If we don't exist (new Game), nothing'll happen
		return;
	if ( this->currentMoves < 2 )
		++this->currentMoves;
	else
		return;
	SDL_LockMutex(move_mutex);
	if ( ! this->alive )
	{
		// We-re dead
		SDL_UnlockMutex(this->move_mutex);
		return;
	}
	bool orientChanged = ( this->orient != direction );
	if ( orientChanged )
		this->orient = direction;
	map::MoveResult moveResult = map::Map::movePlayer(this->coords, direction);
	if ( moveResult == map::NOTHINGHAPPENED && orientChanged )
		Display::movePlayer(this, map::ORIENTCHANGED);
	Display::movePlayer(this, moveResult);
	SDL_UnlockMutex(this->move_mutex);
	--this->currentMoves;
}

void
Player::addToScore(int score)
{
	if ( (this->score += score) < Config::getInt("minimumScore") )
		this->score = -9;
	else if ( this->score > Config::getInt("maximumScore") )
		this->score = 99;
}

void
Player::setPlantableBombs(int plantableBombs)
{
	if ( plantableBombs < Config::getInt("minCapacity") )
		this->plantableBombs = Config::getInt("minCapacity");
	else if ( plantableBombs > Config::getInt("maxCapacity") )
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
	if ( ! this->isAbleToPlantBomb() )
		return;
	if ( bomb::AtomicCenter::plantBomb(this->id, this->coords, this->range) )
	{	// The bomb has been planted !
		++this->plantedBombs;
		Display::plantBomb(this);
	}
}

Player *
Player::playerAt(map::Coords & c)
{
	// We read all players
	for ( std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i )
			// Coords are equal ? That's the one we're looking for !
			if ( (*i)->getCoords() == c )
				return *i;
	return 0;
}

