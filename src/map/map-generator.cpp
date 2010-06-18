/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) mogzor 2010 <mogzor@gmail.com>
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

#include "map-generator.hpp"

using namespace bombherman;
using namespace bombherman::map;

// Initialize statics
bool MapGenerator::isInit = false;

void
MapGenerator::generate(Grid & g)
{
	MapGenerator::init();
	
	// Stock config things to avoid multiple callings to Config
	int insertionProbabilityBaseHorizontal = Config::getInt("mgInsertionProbabilityBaseHorizontal");
	int insertionRegressionHorizontal = Config::getInt("mgInsertionRegressionHorizontal");
	int insertionElementSizeMaxVertical = Config::getInt("mgInsertionElementSizeMaxVertical");
	int insertionRegressionVertical = Config::getInt("mgInsertionRegressionVertical");
	int insertionProbabilityBaseVertical = Config::getInt("mgInsertionProbabilityBaseVertical");
	int insertionElementSizeMaxHorizontal = Config::getInt("mgInsertionElementSizeMaxHorizontal");
	int insertionProbabilityBarrel = Config::getInt("mgInsertionProbabilityBarrel");
	
	// We'll need those local vars for various stocking
	int currentHorizontalElementSize;
	int currentVerticalElementSize(0);
	int currentHorizontalInsertionProbability;
	int currentVerticalInsertionProbability = Config::getInt("mgInsertionProbabilityBaseHorizontal");
	Coords c;
	
	// Resize the map to the right dimensions
	g.grid.resize(g.size);
	for (std::vector< std::vector< char > >::iterator i = g.grid.begin(),
		i_end = g.grid.end() ; i != i_end ; ++i)
	{
		// Fill each cell with NOTHING
		i->resize(g.size, NOTHING);
	}
	
	// Go through the gris to fill it, col by col, line by line
	for (c.x = 0; c.x < g.size; ++c.x)
	{
		for (c.y = 0; c.y < g.size; ++c.y)
		{
			currentHorizontalElementSize = horizontalScan(g, c);
			currentHorizontalInsertionProbability = insertionProbabilityBaseHorizontal - currentHorizontalElementSize * insertionRegressionHorizontal;
			if (currentVerticalElementSize != 0)
			{
				// The upper element is INDESTRUCTIBLE
				if (throwDice(currentVerticalInsertionProbability)
					&& currentVerticalElementSize < insertionElementSizeMaxVertical
					&& testCellLimited(g, c))
				{
					// If we didn't reach max vertical INDESTRUCTIBLE block size and we do not lock anything
					// and the dice tells us to put INDESTRUCTIBLE, juste do it
					g[c.y][c.x] = INDESTRUCTIBLE;
					++currentVerticalElementSize;
					// We'll have less chances to get there next time
					currentVerticalInsertionProbability -= insertionRegressionVertical;
				}
				else
				{
					// Empty cell
					currentVerticalElementSize = 0;
					currentVerticalInsertionProbability = insertionProbabilityBaseVertical;
				}
			}
			else if (currentHorizontalElementSize != 0
				&& currentHorizontalElementSize < insertionElementSizeMaxHorizontal
				&& throwDice(currentHorizontalInsertionProbability)
				&& c.x != (g.size - 1))
					// INDESTRUCTIBLE on left, and can put another one without locking
					g[c.y][c.x] = INDESTRUCTIBLE;
			else if (throwDice(insertionProbabilityBaseVertical)
				&& testCellFull(g, c))
			{
				// Base INDESTRUCTIBLE insertion, none on left nor top
				g[c.y][c.x] = INDESTRUCTIBLE;
				++currentVerticalElementSize;
				currentVerticalInsertionProbability -= insertionRegressionVertical;
			}
		}
	}
	
	// Fill the gris with barrels
	for (c.x = 0; c.x < g.size; ++c.x)
		for (c.y = 0; c.y < g.size; ++c.y)
			if (g[c.y][c.x] == NOTHING
				&& throwDice (insertionProbabilityBarrel))
					g[c.y][c.x] = BARREL;
}

int
MapGenerator::random(int min, int max)
{
	MapGenerator::init();
	
	// Random number between min and max
	return rand() % (max - min + 1) + min;
}

bool
MapGenerator::throwDice(int percentage)
{
	MapGenerator::init();
	
	// percentage % to return true
	return (percentage > rand() % 100);
}

bool
MapGenerator::testCellFull(Grid & grid, Coords & coords)
{
	if (coords.x == 0)
		// Don't care about first column
		return true;
	bool result = grid[coords.y][coords.x - 1] != INDESTRUCTIBLE;
	if (coords.y > 0)
		result = result && grid[coords.y - 1][coords.x - 1] != INDESTRUCTIBLE &&
			grid[coords.y - 1][coords.x] != INDESTRUCTIBLE;
	if (coords.y < grid.size - 1)
		result = result && grid[coords.y+1][coords.x-1] != INDESTRUCTIBLE;
	// We'll return false if a nearby cell is INDESTRUCTIBLE (strict north and all west)
	return result;
}

bool
MapGenerator::testCellLimited(Grid & grid, Coords & coords)
{
	// Return true if the south west cell is INDESTRUCTIBLE or out of map
	return (coords.y == grid.size - 1 || coords.x == 0 ||
		grid[coords.y + 1][coords.x - 1] != INDESTRUCTIBLE);
}

int
MapGenerator::horizontalScan(Grid & grid, Coords & coords)
{
	int i;
	for (i = 0 ; coords.x - i != 0
		&& grid[coords.y][coords.x - i - 1] == INDESTRUCTIBLE ; ++i);
	// Return the number of cell before next not INDESTRUCTIBLE on the left
	return i;
}

Coords
MapGenerator::getRandomCoords()
{
	MapGenerator::init();
	
	// Return random coords
	Coords c;
	c.x = static_cast<Uint32>(MapGenerator::random(0, c.max));
	c.y = static_cast<Uint32>(MapGenerator::random(0, c.max));
	return c;
}

void
MapGenerator::init()
{
	if (MapGenerator::isInit)
		// Don't init twice
		return;
	::srand(time(0));
	MapGenerator::isInit = true;
}

