/* 
 * File:   Player.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 11:52 AM
 */

#include "player.hpp"

using namespace bombherman;

Player::Player()
{
	this->plantableBombs = Config::getInt("defaultPlantableBombs");
	this->range = Config::getInt("defaultRange");
	this->plantedBombs = 0;
	this->score = 0;
}

int
Player::getRange()
{
	return this->range;
}

map::Coords *
Player::getCoords()
{
	return this->coords;
}

bool
Player::isAbleToPlantBomb()
{
  return (this->plantableBombs < this->plantedBombs);
}

int
Player::getScore()
{
	return this->score;
}

void
Player::setRange(int range)
{
	this->range = range;
}

void
Player::goLeft()
{
}

void
Player::goRight()
{
}

void
Player::goTop()
{
}

void
Player::goBottom()
{
}

void
Player::plantBomb()
{
}

void
Player::addToScore(int score)
{
	this->score += int score;
}

void
Player::setPlantableBombs(int plantableBombs)
{
	this->plantableBombs = int plantableBombs;
}

void
Player::addToPlantableBombs(int plantableBombs)
{
	this->plantableBombs += int plantableBombs;
}

void
Player::resetToDefaultStats()
{
	this->plantableBombs = Config::getInt("defaultPlantableBombs");
	this->range = Config::getInt("defaultRange");
	this->plantedBombs = 0;
}