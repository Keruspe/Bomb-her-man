/* 
 * File:   Player.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 11:52 AM
 */

#include "player.hpp"
#include "map/map.hpp"
#include "exceptions/too-many-players-exception.hpp"
#include "atomic-center/atomic-center.hpp"
#include "game.hpp"

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

std::vector< Player * > &
Player::getPlayers()
{
	return Player::players;
}

Player *
Player::getPlayer(int playerNo)
{
	if (Player::players.size() < static_cast<unsigned>(playerNo)
		|| 0 >= playerNo)
			return 0;
	return Player::players[playerNo - 1];
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

void
Player::die()
{
	++Player::icyDeadPeople;
	map::Map::removePlayer(&this->coords);
	if (Player::players.size() - Player::icyDeadPeople > 1)
		this->alive = false;
	else
		Player::reInit();
}

void
Player::reInit()
{
	bomb::AtomicCenter::boum();
	Player::icyDeadPeople = 0;
	for (std::vector< Player * > ::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i)
			(*i)->alive = true;
	/* TODO : End of game */
	Game::nextMap();
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
	if (! this->alive)
		return map::NOTHINGHAPPENED;
	bool orientChanged = false;
	if (this->orient != direction)
	{
		orientChanged = true;
		this->orient = direction;
	}
	map::MoveResult moveResult = map::Map::movePlayer(&this->coords, direction);
	if (moveResult == map::NOTHINGHAPPENED && orientChanged)
		return map::ORIENTCHANGED;
	return moveResult;
}

void
Player::addToScore(int score)
{
	this->score += score;
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
}

void
Player::plantBomb()
{
	bomb::AtomicCenter::plantBomb(this);
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
	{
		if((*i)->getCoords().x == c.x && (*i)->getCoords().y == c.y)
			return *i;
	}
	return 0;
}

bool
Player::isAlive()
{
	return this->alive;
}

