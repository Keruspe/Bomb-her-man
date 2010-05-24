/* 
 * File:   Player.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 11:52 AM
 */

#include "player.hpp"

using namespace bombherman;
using namespace bombherman::map;

int Player::maxId = 0;

Player::Player()
{

	this->plantableBombs = Config::getInt("defaultPlantableBombs");
	this->range = Config::getInt("defaultRange");
	this->plantedBombs = 0;
	this->score = 0;
	this->id = ++ Player::maxId;
}

int
Player::getRange()
{
	return this->range;
}

Coords
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

void
Player::setCoords(Coords & c)
{
	this->coords = c;
}

void
Player::setRange(int range)
{
	this->range = range;
}

void
Player::go(Direction direction)
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
