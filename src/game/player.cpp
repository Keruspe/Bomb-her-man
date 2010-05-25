/* 
 * File:   Player.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 11:52 AM
 */

#include "player.hpp"
#include "exceptions/too-many-players-exception.hpp"

using namespace bombherman;

std::vector<Player * > Player::players;

Player::Player() : plantableBombs (Config::getInt("defaultPlantableBombs")),
		range (Config::getInt("defaultRange")),
		plantedBombs (0),
		score (0),
		id (Player::players.size() + 1),
		coords (map::Coords())
{
	Player::players.push_back(this);
}

Player::~Player()
{
	for (std::vector< Player * >::iterator i = Player::players.begin(), i_end = Player::players.end(); i != i_end ; ++i)
	{
		delete(*i);
	}
	Player::players.clear();
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

Player &
Player::getPlayer(int playerNo)
{
	return *(Player::players[playerNo - 1]);
}

void
Player::newPlayer()
{
	if (Player::players.size() >= static_cast<unsigned>(Config::getInt("maxPlayers")))
		throw exceptions::TooManyPlayersException();
	Player::players.push_back(new Player());
}

void
Player::setRange(int range)
{
	this->range = range;
}

void
Player::go(map::Direction direction)
{

}

void
Player::addToScore(int score)
{
	this->score += score;
}

void
Player::setPlantableBombs(int plantableBombs)
{
	this->plantableBombs =plantableBombs;
}

void
Player::addToPlantableBombs(int plantableBombs)
{
	this->plantableBombs += plantableBombs;
}

void
Player::resetToDefaultStats()
{
	this->plantableBombs = Config::getInt("defaultPlantableBombs");
	this->range = Config::getInt("defaultRange");
	this->plantedBombs = 0;
}

void
Player::setCoords(map::Coords & c)
{
	this->coords = c;
}
