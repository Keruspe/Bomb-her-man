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

#include "bombherman.hpp"
#include "game/game.hpp"
#include "map/map.hpp"
#include "map/map-utils.hpp"
#include "map-generator/map-generator.hpp"

using namespace bombherman;

int
main(int argc, char *argv[])
{
	Config::set("mapSize", 15);
	map::Map map;
	//map::Map map("/home/keruspe/map.bm");
	map::Coords coords;
	int size = Config::getInt("mapSize");
	for(coords.y = 0 ; coords.y < size ; ++coords.y)
	{
		for(coords.x = 0 ; coords.x < size ; ++coords.x)
		{
			std::cout << map.get(coords);
		}
		std::cout << std::endl;
	}
	
	Game::main();
	
	return 0;
}
