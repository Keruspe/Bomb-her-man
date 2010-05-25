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

Map::Map() : map(Grid())
{
	MapGenerator::generate(map);
}

Map::Map(Grid & model) : map(Grid())
{
	for(int x(0) ; x < map.size ; ++x)
	{
		for(int y(0) ; y < map.size ; ++y)
		{
			map[y][x] = model[y][x];
		}
	}
}

Map::Map(std::string path) : map(Grid())
{
	try
	{
		if (! MapParser::parse(path, map))
		{
			std::cerr << "The file in which the program looked "
				<< "for the map was malformed." << std::endl;
			throw MalformedFileException(path);
		}
	}
	catch(BadElementException & e)
	{
		std::cerr << "An error has been detected in " << path
			<< " : '" + e.message() + "'." << std::endl;
	}
}

Map::~Map()
{
}

void
Map::placePlayers()
{
	Coords c;
	for(std::vector< Player * >::iterator i = Player::getPlayers().begin(), i_end = Player::getPlayers().end() ; i != i_end ; ++i)
	{
		while (true)
		{
			c = MapGenerator::getRandomCoords();
			if(isPlayer(map[c.y][c.x])
				|| (c.y != 0 && isPlayer(map[c.y - 1][c.x]))
				|| (c.y != map.size - 1 && isPlayer(map[c.y+1][c.x]))
				|| (c.x != 0 && isPlayer(map[c.y][c.x - 1]))
				|| (c.x != map.size - 1 && isPlayer(map[c.y][c.x + 1])))
					continue;
			if (c.y != 0)
				map[c.y - 1][c.x] = NONE;
			if (c.y != map.size - 1)
				map[c.y + 1][c.x] = NONE;
			if (c.x != 0)
				map[c.y][c.x - 1] = NONE;
			if (c.x != map.size - 1)
				map[c.y][c.x + 1] = NONE;
			(*i)->setCoords(c);
			map[c.y][c.x] = (*i)->getId() + '0';
			break;
		}
	}
}

bool
Map::isPlayer(char playerNo)
{
	return (playerNo >= '1' && playerNo <= '0' + Config::getInt("maxPlayers"));
}

char
Map::get(Coords c)
{
	return map[c.y][c.x];
}

Coords &
Map::getCoords(int playerNo)
{
	Coords *coords = new Coords();
	for(coords->y = 0 ; coords->y < map.size ; ++coords->y)
	{
		for(coords->x = 0 ; coords->x < map.size ; ++coords->x)
		{
			if (this->get(*coords) == playerNo + '0')
				return *coords;
		}
	}
	coords->x = -1;
	coords->y = -1;
	return *coords;
}

bool
Map::movePlayer(Player * player, Direction & direction)
{
	switch(direction)
	{
	case UP:
		break;
	case DOWN:
		break;
	case LEFT:
		break;
	case RIGHT:
		break;
	}
	return false;
}

