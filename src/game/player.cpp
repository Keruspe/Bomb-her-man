/* 
 * File:   Player.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 11:52 AM
 */

#include <SDL_thread.h>

#include "player.hpp"
#include "game.hpp"
#include "exceptions/too-many-players-exception.hpp"
#include "atomic-center/atomic-center.hpp"

using namespace bombherman;

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

Player::~Player()
{
}

int
Player::getRange()
{
	return this->range;
}

map::Coords &
Player::getCoords()
{
	return this->coords;
}

map::Direction &
Player::getOrient()
{
	return this->orient;
}

bool
Player::isAbleToPlantBomb()
{
	return (this->plantableBombs > this->plantedBombs);
}

int
Player::getScore()
{
	return this->score;
}

int
Player::getId ()
{
	return this->id;
}

Player *
Player::getPlayer(int id)
{
	if (Player::players.size() < static_cast<unsigned>(id)
		|| 0 >= id)
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
	{
		SDL_CreateThread(Player::reInit, NULL);
		return true;
	}
}

int
Player::reInit(void * dummy)
{
	bomb::AtomicCenter::boum();
	Player::icyDeadPeople = 0;
	for (std::vector< Player * > ::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i)
			(*i)->resetToDefaultStats();
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

void
Player::addToRange(int range)
{
	this->setRange(this->range + range);
}

map::MoveResult
Player::go(map::Direction & direction)
{
	if (! this || ! this->alive)
		return map::NOTHINGHAPPENED;
	bool orientChanged(false);
	if (this->orient != direction)
	{
		orientChanged = true;
		this->orient = direction;
	}
	map::MoveResult moveResult = map::Map::movePlayer(this->coords, direction);
	if (moveResult == map::NOTHINGHAPPENED && orientChanged)
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
Player::addToPlantableBombs(int plantableBombs)
{
	this->setPlantableBombs(this->plantableBombs + plantableBombs);
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
	bomb::AtomicCenter::plantBomb(this->id, this->coords);
	++this->plantedBombs;
}

void
Player::setCoords(map::Coords & c)
{
	this->coords = c;
}

Player *
Player::playerAt(map::Coords & c)
{
	for (std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i)
			if((*i)->getCoords().x == c.x && (*i)->getCoords().y == c.y)
				return *i;
	return 0;
}

bool
Player::isAlive()
{
	return this->alive;
}

void
Player::bombHasExploded()
{
	--this->plantedBombs;
}

