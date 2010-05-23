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

#define INSERTION_PROBABILITY_BASE 1
#define INSERTION_PROBABILITY_BASE_VERTICAL 0.5
#define INSERTION_PROBABILITY_BASE_HORIZONTAL 0.5
#define INSERTION_PROBABILITY_REGRESSION_VERTICAL 0.1
#define INSERTION_PROBABILITY_REGRESSION_HORIZONTAL 0.1
#define INSERTION_ELEMENT_SIZE_MAX_VERTICAL 3
#define INSERTION_ELEMENT_SIZE_MAX_HORIZONTAL 3

using namespace bombherman;
using namespace bombherman::map;

void
MapGenerator::generate(Grid& g)
{
    ::srand(time(0));
    int currentHorizontalElementSize, currentVerticalElementSize = 0;
    float currentHorizontalInsertionProbability, currentVerticalInsertionProbability = INSERTION_PROBABILITY_BASE_VERTICAL;
    Coords c;
    std::vector<char> *current_line;
    for (c.x = 0; c.x < g.size; ++c.x)
    {
        current_line = new std::vector<char>();
        for (c.y = 0; c.y < g.size; ++c.y)
        {
            currentHorizontalElementSize = horizontalScan(g, c);
            currentHorizontalInsertionProbability = INSERTION_PROBABILITY_BASE_HORIZONTAL - currentHorizontalElementSize * INSERTION_PROBABILITY_REGRESSION_HORIZONTAL;
            if (currentVerticalElementSize != 0)
            {
                if (throwDice(currentVerticalInsertionProbability) && currentVerticalElementSize < INSERTION_ELEMENT_SIZE_MAX_VERTICAL && testCellLimited(g, c))
                {
                    current_line->push_back(INDESTRUCTIBLE);
                    currentVerticalElementSize ++;
                    currentVerticalInsertionProbability -= INSERTION_PROBABILITY_REGRESSION_VERTICAL;
                }
                else
                {
                    current_line->push_back(NONE);
                    currentVerticalElementSize = 0;
                    currentVerticalInsertionProbability = INSERTION_PROBABILITY_BASE_VERTICAL;
                }
            }
            else if (currentHorizontalElementSize != 0 && currentHorizontalElementSize < INSERTION_ELEMENT_SIZE_MAX_HORIZONTAL && throwDice(currentHorizontalInsertionProbability) && c.x != 14)
            {
                    current_line->push_back(INDESTRUCTIBLE);
            }
            else if (throwDice(INSERTION_PROBABILITY_BASE) && testCellFull(g, c))
            {
                current_line->push_back(INDESTRUCTIBLE);
                currentVerticalElementSize ++;
                currentVerticalInsertionProbability -= INSERTION_PROBABILITY_REGRESSION_VERTICAL;
            }
            else
            {
                current_line->push_back(NONE);
            }
        }
        g.grid.push_back(*current_line);
        delete(current_line);
    }
}

int
MapGenerator::random(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool
MapGenerator::throwDice(float probability)
{
    float random = (rand() % 100) / 100.;
    if (probability > random)
        return true;
    return false;
}

bool
MapGenerator::testCellFull(Grid& grid, Coords coords)
{
    bool result = true;
    if (coords.y != 0)
        result = result &&
            grid.grid[coords.y - 1][coords.x - 1] != INDESTRUCTIBLE &&
            grid.grid[coords.y - 1][coords.x] != INDESTRUCTIBLE;
    if (coords.x != 0)
    {
        if (coords.y != grid.size - 1)
            result = result && grid.grid[coords.y + 1][coords.x - 1] != INDESTRUCTIBLE;
        result = result && grid.grid[coords.y][coords.x - 1] != INDESTRUCTIBLE;
    }
    return result;
}

bool
MapGenerator::testCellLimited(Grid& grid, Coords coords)
{
    bool result = true;
    if (coords.y != grid.size - 1)
        result = result && grid.grid[coords.y + 1][coords.x - 1] != INDESTRUCTIBLE;
    return result;
}

int
MapGenerator::horizontalScan(Grid& grid, Coords coords)
{
    int i = 0;
    if (coords.x == 0)
        return 0;
    while(grid.grid[coords.y][coords.x - i - 1] == INDESTRUCTIBLE)
    {
        i ++;
        if (coords.x - i == 0)
            return i;
    }
    return i;
}
