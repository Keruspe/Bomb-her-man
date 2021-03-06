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
#include "map-generator.hpp"
#include "bombherman.hpp"

using namespace bombherman;
using namespace bombherman::map;

// Initialize statics
Grid Map::map;

void
Map::newMap()
{
	Map::map = Grid();
	
	// The default map creator calls the MapGenerator
	MapGenerator::generate(Map::map);
	Map::map.exists = true;
	Map::placePlayers();
}

void
Map::newMap(std::string path)
{
	try
	{
		// This map creator calls the MapParser
		if ( ! MapParser::parse(path, Map::map) )
		{
			bherr << "The file in which the program looked "
				<< "for the map was malformed." << bhendl;
			throw exceptions::map::MalformedFileException(path);
		}
	}
	catch (exceptions::map::BadElementException & e)
	{
		bherr << "An error has been detected in " << path << bhendl;
		throw e;
	}
	Map::map.exists = true;
	Map::placePlayers();
}

void
Map::deleteMap()
{
	Map::map.exists = false;
	for ( std::vector< std::vector< char > >::iterator i = Map::map.grid.begin(),
		i_end = Map::map.grid.end() ; i != i_end ; ++i )
			i->clear();
	Map::map.grid.clear();
}

void
Map::placePlayers()
{
	Coords c;
	for ( std::vector< Player * >::iterator i = Player::players.begin(),
		i_end = Player::players.end() ; i != i_end ; ++i )
	{
		while ( true )
		{
			// We get random coords from MapGenerator
			c = MapGenerator::getRandomCoords();
			
			// We check that we are not on a cell stuck at another player
			if ( (Map::map[c.y][c.x] == PLAYER)
				|| (c.y != 0 && Map::map[c.y - 1][c.x] == PLAYER)
				|| (c.y != Map::map.size - 1 && Map::map[c.y+1][c.x] == PLAYER)
				|| (c.x != 0 && Map::map[c.y][c.x-1] == PLAYER)
				|| (c.x != Map::map.size - 1 && Map::map[c.y][c.x+1] == PLAYER) )
					continue;
			
			// We clear the map around the player
			if ( c.y != 0 )
				Map::map[c.y - 1][c.x] = NOTHING;
			if ( c.y != Map::map.size - 1 )
				Map::map[c.y + 1][c.x] = NOTHING;
			if ( c.x != 0 )
				Map::map[c.y][c.x - 1] = NOTHING;
			if ( c.x != Map::map.size - 1 )
				Map::map[c.y][c.x + 1] = NOTHING;
			
			// We tell the player where he is
			(*i)->setCoords(c);
			Map::map[c.y][c.x] = PLAYER;
			break;
		}
	}
}

bool
Map::plantBomb(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() || Map::get(c) != PLAYER )
		// Ok that's weird, only a valid player should be able to do that
		return false;
	
	// The bomb has been planted !
	Map::map[c.y][c.x] = PLAYONBOMB;
	return true;
}

char
Map::get(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() )
		return 0;
	return Map::map[c.y][c.x];
}

char
Map::get(Uint32 x, Uint32 y)
{
	if ( ! Map::exists() || ! Coords(x, y).validate() )
		return 0;
	return Map::map[y][x];
}

MoveResult
Map::movePlayer( Coords & c, Direction & direction )
{
	if ( ! Map::exists() )
		return NOTHINGHAPPENED;
	
	// Used to stock whether we moved or not
	bool tmpBool;
	switch ( direction )
	{
	case UP:
		tmpBool = Map::moveUp(c);
		break;
	case DOWN:
		tmpBool = Map::moveDown(c);
		break;
	case LEFT:
		tmpBool = Map::moveLeft(c);
		break;
	case RIGHT:
		tmpBool = Map::moveRight(c);
		break;
	}
	if ( ! tmpBool )
		return NOTHINGHAPPENED;
	if ( Map::map[c.y][c.x] != BOMB )
	{
		// Now used to stock if we took a bonus
		tmpBool = Map::applyBonus(c);
		Map::map[c.y][c.x] = PLAYER;
		if ( tmpBool )
			return BONUSTAKEN;
	}
	else
		// We're on a bomb, we'd better run
		Map::map[c.y][c.x] = PLAYONBOMB;
	return MOVED;
}

bool
Map::moveUp(Coords & c)
{
	if ( c.y <= 0 || Map::map[c.y - 1][c.x] == BARREL
		|| Map::map[c.y - 1][c.x] == INDESTRUCTIBLE
		|| Map::map[c.y - 1][c.x] == PLAYONBOMB
		|| Map::map[c.y - 1][c.x] == PLAYER )
			return false;
	Map::cleanOldSpot(c);
	--c.y;
	return true;
}

bool
Map::moveDown(Coords & c)
{
	if ( c.y >= (Map::map.size - 1) || Map::map[c.y + 1][c.x] == BARREL
		|| Map::map[c.y + 1][c.x] == INDESTRUCTIBLE
		|| Map::map[c.y + 1][c.x] == PLAYONBOMB
		|| Map::map[c.y + 1][c.x] == PLAYER )
			return false;
	Map::cleanOldSpot(c);
	++c.y;
	return true;
}

bool
Map::moveLeft(Coords & c)
{
	if ( c.x <= 0 || Map::map[c.y][c.x - 1] == BARREL
		|| Map::map[c.y][c.x - 1] == INDESTRUCTIBLE
		|| Map::map[c.y][c.x - 1] == PLAYONBOMB
		|| Map::map[c.y][c.x - 1] == PLAYER )
			return false;
	Map::cleanOldSpot(c);
	--c.x;
	return true;
}

bool
Map::moveRight(Coords & c)
{
	if ( c.x >= (Map::map.size - 1) || Map::map[c.y][c.x + 1] == BARREL
		|| Map::map[c.y][c.x + 1] == INDESTRUCTIBLE
		|| Map::map[c.y][c.x + 1] == PLAYONBOMB
		|| Map::map[c.y][c.x + 1] == PLAYER )
			return false;
	Map::cleanOldSpot(c);
	++c.x;
	return true;
}

void
Map::cleanOldSpot(Coords & c)
{
	if ( Map::map[c.y][c.x] == PLAYONBOMB )
		Map::map[c.y][c.x] = BOMB;
	else if ( Map::map[c.y][c.x] != BOMB )
		Map::map[c.y][c.x] = NOTHING;
}

void
Map::destroyBarrel(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() || Map::get(c) != BARREL )
		return;
	
	// Was there a bonus in this one ? :-)
	if ( MapGenerator::throwDice(Config::getInt("bonusApparitionProbability")) )
		Map::map[c.y][c.x] = FIRSTBONUS + MapGenerator::random(0, LASTBONUS-FIRSTBONUS);
	else
		Map::map[c.y][c.x] = NOTHING;
}

void
Map::removePlayer(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() )
		return;
	if ( Map::get(c) == PLAYER )
		Map::map[c.y][c.x] = NONE;
	else if ( Map::get(c) == PLAYONBOMB )
		Map::map[c.y][c.x] = BOMB;
	
	// Set coords to any invalid thing
	c.x = Config::getInt("mapSize");
	c.y = Config::getInt("mapSize");
}

void
Map::removeBomb(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() )
		return;
	if ( Map::get(c) == BOMB )
		Map::map[c.y][c.x] = NONE;
	else if ( Map::get(c) == PLAYONBOMB )
		Map::map[c.y][c.x] = PLAYER;
}

void
Map::removeBonus(Coords & c)
{
	if ( ! Map::exists() || ! c.validate() )
		return;
	Map::map[c.y][c.x] = NONE;
}

bool
Map::applyBonus(Coords & c)
{
	Player * player = Player::playerAt(c);
	if ( player == 0 )
		// No player there, weird...
		return false;
	int variation(1);
	switch ( static_cast< Bonus >(Map::map[c.y][c.x]) )
	{
	case FIREDOWN:
		// Will be negative
		variation = -1;
	case FIREUP:
		player->addToRange(variation * Config::getInt("rangeVariation"));
		break;
	case NULLFIRE:
		player->setRange(Config::getInt("minRange"));
		break;
	case FULLFIRE:
		player->setRange(Config::getInt("maxRange"));
		break;
	case BOMBDOWN:
		// Will ve negative
		variation = -1;
	case BOMBUP:
		player->addToPlantableBombs(variation * Config::getInt("capacityVariation"));
		break;
	default:
		return false;
	}
	return true;
}

void
Map::toString()
{
	if ( ! Map::exists() )
		return;
	// Read the lines
	for ( std::vector< std::vector< char > >::iterator i = Map::map.grid.begin(),
		i_end = Map::map.grid.end() ; i != i_end ; ++i )
	{
		// Read the cols
		for ( std::vector< char >::iterator j = i->begin(), j_end = i->end() ;
			j != j_end ; ++j )
		{
			// Print the current cell
			std::cout << '[' << *j << ']';
		}
		std::cout << std::endl;
	}
}

