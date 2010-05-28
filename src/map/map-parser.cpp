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

#include <fstream>
#include <string>

#include "map-parser.hpp"

using namespace bombherman;
using namespace bombherman::map;

bool
MapParser::parse(std::string path, Grid & map)
{
	std::fstream file(path.c_str(), std::ios::in);
	std::string line;
	char c;
	std::vector<char> *current_line;
	for(int y(0) ; std::getline(file, line) && y < map.size ; ++y)
	{
		current_line = new std::vector<char>();
		for(int x(0) ; x < map.size ; ++x)
		{
			try
			{
				switch(c = line.at(x))
				{
					case NOTHING:
					case BARREL:
					case INDESTRUCTIBLE:
					case BOMB:
						current_line->push_back(c);
						break;
					default:
						char elem[2] = {c, '\0'};
						throw exceptions::BadElementException(elem);
				}
			}
			catch (std::exception)
			{
				return false;
			}
		}
		map.grid.push_back(*current_line);
		delete(current_line);
	}
	return (map.grid.size() == static_cast<unsigned>(map.size));
}

