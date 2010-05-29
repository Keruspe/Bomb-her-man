/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) mogzor 2010 <mogzor@gmail.com>
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

void
MapGenerator::generate(Grid & g)
{
	::srand(time(0));
	int currentHorizontalElementSize, currentVerticalElementSize = 0;
	int currentHorizontalInsertionProbability;
	int currentVerticalInsertionProbability = Config::getInt("mgInsertionProbabilityBaseHorizontal");
	Coords c;
	g.grid.resize(g.size);
	for (std::vector< std::vector< char > >::iterator i = g.grid.begin(),
		i_end = g.grid.end() ; i != i_end ; ++i)
	{
		i->resize(g.size, NOTHING);
	}
	for (c.x = 0; c.x < g.size; ++c.x)
	{
		for (c.y = 0; c.y < g.size; ++c.y)
		{
			currentHorizontalElementSize = horizontalScan(g, c);
			currentHorizontalInsertionProbability = Config::getInt("mgInsertionProbabilityBaseHorizontal") - currentHorizontalElementSize * Config::getInt("mgInsertionRegressionHorizontal");
			if (currentVerticalElementSize != 0)
			{
				if (throwDice(currentVerticalInsertionProbability)
					&& currentVerticalElementSize < Config::getInt("mgInsertionElementSizeMaxVertical")
					&& testCellLimited(g, c))
				{
					g[c.y][c.x] = INDESTRUCTIBLE;
					++currentVerticalElementSize;
					currentVerticalInsertionProbability -= Config::getInt("mgInsertionRegressionVertical");
				}
				else
				{
					currentVerticalElementSize = 0;
					currentVerticalInsertionProbability = Config::getInt("mgInsertionProbabilityBaseVertical");
				}
			}
			else if (currentHorizontalElementSize != 0
				&& currentHorizontalElementSize < Config::getInt("mgInsertionElementSizeMaxHorizontal")
				&& throwDice(currentHorizontalInsertionProbability)
				&& c.x != 14)
			{
				g[c.y][c.x] = INDESTRUCTIBLE;
			}
			else if (throwDice(Config::getInt("mgInsertionProbabilityBaseVertical"))
				&& testCellFull(g, c))
			{
				g[c.y][c.x] = INDESTRUCTIBLE;
				++currentVerticalElementSize;
				currentVerticalInsertionProbability -= Config::getInt("mgInsertionRegressionVertical");
			}
		}
	}
	for (c.x = 0; c.x < g.size; ++c.x)
	{
		for (c.y = 0; c.y < g.size; ++c.y)
		{
			if (g[c.y][c.x] == NOTHING
				&& throwDice (Config::getInt("mgInsertionProbabilityBarrel")))
					g[c.y][c.x] = BARREL;
		}
	}
}

int
MapGenerator::random(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

bool
MapGenerator::throwDice(int percentage)
{
	return (percentage > rand() % 100);
}

bool
MapGenerator::testCellFull(Grid & grid, Coords & coords)
{
	if (coords.x == 0)
		return true;
	bool result = grid[coords.y][coords.x - 1] != INDESTRUCTIBLE;
	if (coords.y > 0)
		result = result && grid[coords.y - 1][coords.x - 1] != INDESTRUCTIBLE &&
			grid[coords.y - 1][coords.x] != INDESTRUCTIBLE;
	if (coords.y < grid.size - 1)
		result = result && grid[coords.y+1][coords.x-1] != INDESTRUCTIBLE;
	return result;
}

bool
MapGenerator::testCellLimited(Grid & grid, Coords & coords)
{
	return (coords.y == grid.size - 1 || coords.x == 0 ||
		grid[coords.y + 1][coords.x - 1] != INDESTRUCTIBLE);
}

int
MapGenerator::horizontalScan(Grid & grid, Coords & coords)
{
	int i;
	for (i = 0 ; coords.x - i != 0
		&& grid[coords.y][coords.x - i - 1] == INDESTRUCTIBLE ; ++i);
	return i;
}

Coords
MapGenerator::getRandomCoords()
{
	Coords c;
	c.x = static_cast<Uint32>(MapGenerator::random(0, c.max));
	c.y = static_cast<Uint32>(MapGenerator::random(0, c.max));
	return c;
}

