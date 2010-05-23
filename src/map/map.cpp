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
			map.grid[y][x] = model.grid[y][x];
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

bool
Map::move(Coords c, Direction d)
{
	return false;
}

void
Map::put(Coords c, Element e)
{
}

char
Map::get(Coords c)
{
	return map.grid[c.y][c.x];
}
