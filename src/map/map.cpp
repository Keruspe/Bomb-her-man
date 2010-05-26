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

#include "map.hpp"
#include "map-generator/map-generator.hpp"

using namespace bombherman;
using namespace bombherman::map;
using namespace bombherman::exceptions;

Grid Map::map;

Map::Map()
{
	MapGenerator::generate(Map::map);
}

Map::Map(Grid & model)
{
	for(int x(0) ; x < Map::map.size ; ++x)
	{
		for(int y(0) ; y < Map::map.size ; ++y)
		{
			Map::map[y][x] = model[y][x];
		}
	}
}

Map::Map(std::string path)
{
	try
	{
		if (! MapParser::parse(path, Map::map))
		{
			std::cerr << "The file in which the program looked "
				<< "for the map was malformed." << std::endl;
			throw MalformedFileException(path);
		}
	}
	catch(BadElementException & e)
	{
		std::cerr << "An error has been detected in " << path << std::endl;
		throw e;
	}
}

Map::~Map()
{
}

void
Map::placePlayers()
{
	Coords c;
	for(std::vector< Player * >::iterator i = Player::getPlayers().begin(),
		i_end = Player::getPlayers().end() ; i != i_end ; ++i)
	{
		while (true)
		{
			c = MapGenerator::getRandomCoords();
			if((Map::map[c.y][c.x] == PLAYER)
				|| (c.y != 0 && Map::map[c.y - 1][c.x] == PLAYER)
				|| (c.y != Map::map.size - 1 && Map::map[c.y+1][c.x] == PLAYER)
				|| (c.x != 0 && Map::map[c.y][c.x-1] == PLAYER)
				|| (c.x != Map::map.size - 1 && Map::map[c.y][c.x+1] == PLAYER))
					continue;
			if (c.y != 0)
				Map::map[c.y - 1][c.x] = NOTHING;
			if (c.y != Map::map.size - 1)
				Map::map[c.y + 1][c.x] = NOTHING;
			if (c.x != 0)
				Map::map[c.y][c.x - 1] = NOTHING;
			if (c.x != Map::map.size - 1)
				Map::map[c.y][c.x + 1] = NOTHING;
			(*i)->setCoords(c);
			Map::map[c.y][c.x] = PLAYER;
			break;
		}
	}
}

bool
Map::plantBomb(Coords & c)
{
	if (0 > c.x || 0 > c.y || Map::map.size <= c.y
		|| Map::map.size <= c.x || Map::get(c) != NOTHING)
			return false;
	Map::map[c.y][c.x] = BOMB;
	return true;
}

char
Map::get(Coords c)
{
	if (0 > c.x || 0 > c.y || Map::map.size <= c.y || Map::map.size <= c.x)
		return 0;
	return Map::map[c.y][c.x];
}

char
Map::get(int x, int y)
{
	if (0 > x || 0 > y || Map::map.size <= y || Map::map.size <= x)
		return 0;
	return Map::map[y][x];
}

bool
Map::movePlayer(Coords * coords, Direction & direction)
{
	bool move;
	switch(direction)
	{
	case UP:
		move = Map::moveUp(coords);
		break;
	case DOWN:
		move = Map::moveDown(coords);
		break;
	case LEFT:
		move = Map::moveLeft(coords);
		break;
	case RIGHT:
		move = Map::moveRight(coords);
		break;
	}
	if (! move)
		return false;
	Map::applyBonus(coords);
	Map::map[coords->y][coords->x] = PLAYER;
	return true;
}

bool
Map::moveUp(Coords * c)
{
	if (c->y <= 0 || Map::map[c->y - 1][c->x] == BARREL
		|| Map::map[c->y - 1][c->x] == INDESTRUCTIBLE
		|| Map::map[c->y - 1][c->x] == PLAYER)
		return false;
	Map::map[c->y][c->x] = NOTHING;
	--c->y;
	return true;
}

bool
Map::moveDown(Coords * c)
{
	if (c->y >= (Map::map.size - 1) || Map::map[c->y + 1][c->x] == BARREL
		|| Map::map[c->y + 1][c->x] == INDESTRUCTIBLE
		|| Map::map[c->y + 1][c->x] == PLAYER)
		return false;
	Map::map[c->y][c->x] = NOTHING;
	++c->y;
	return true;
}

bool
Map::moveLeft(Coords * c)
{
	if (c->x <= 0 || Map::map[c->y][c->x - 1] == BARREL
		|| Map::map[c->y][c->x - 1] == INDESTRUCTIBLE
		|| Map::map[c->y][c->x - 1] == PLAYER)
		return false;
	Map::map[c->y][c->x] = NOTHING;
	--c->x;
	return true;
}

bool
Map::moveRight(Coords * c)
{
	if (c->x >= (Map::map.size - 1) || Map::map[c->y][c->x + 1] == BARREL
		|| Map::map[c->y][c->x + 1] == INDESTRUCTIBLE
		|| Map::map[c->y][c->x + 1] == PLAYER)
		return false;
	Map::map[c->y][c->x] = NOTHING;
	++c->x;
	return true;
}

void
Map::destroy(Coords & c)
{
	if (0 > c.x || 0 > c.y || Map::map.size <= c.y
		|| Map::map.size <= c.x || Map::get(c) != BARREL)
			return;
	if (MapGenerator::throwDice(Config::getInt("bonusApparitionProbability")))
		Map::map[c.y][c.x] = BOMBUP + MapGenerator::random(0, NULLFIRE-BOMBUP);
	else
		Map::map[c.y][c.x] = NOTHING;
}

void
Map::applyBonus(Coords * c)
{
	Player * player = Player::playerAt(c);
	if (player == 0)
		return;
	int variation(1);
	switch(static_cast<Bonus>(Map::map[c->y][c->x]))
	{
	case NONE:
		break;
	case NULLFIRE:
		variation *= 0; // will be minored by minRange
	case FIREDOWN:
		variation *= -1;
	case FIREUP:
		player->addToRange(variation * Config::getInt("rangeVariation"));
		break;
	case FULLFIRE:
		player->setRange(Config::getInt("maxRange"));
		break;
	case BOMBDOWN:
		variation *= -1;
	case BOMBUP:
		player->addToPlantableBombs(variation * Config::getInt("capacityVariation"));
		break;
	}
}

