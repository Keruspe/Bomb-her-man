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

#define VERT_HOR_RATIO 0.5
#define INSERTION_PROBABILITY_BASE_VERTICAL 0.6
#define INSERTION_PROBABILITY_BASE_HORIZONTAL 0.6
#define INSERTION_PROBABILITY_REGRESSION_VERTICAL 0.1
#define INSERTION_PROBABILITY_REGRESSION_HORIZONTAL 0.1
#define INSERTION_ELEMENT_SIZE_MAX_VERTICAL 7
#define INSERTION_ELEMENT_SIZE_MAX_HORIZONTAL 7

void MapGenerator::generate(Grid& g) {
    int i, j = 0;
    for (i = 0; i < g.size; i++) {
        for (j = 0; j < g.size; j++) {
            g.grid[i][j] = NONE;
        }
    }
    ::srand(time(0));
    int currentVerticalElementSize = 0;
    int currentHorizontalElementSize = 0;
    float currentVerticalInsertionProbability = INSERTION_PROBABILITY_BASE_VERTICAL;
    float currentHorizontalInsertionProbability = INSERTION_PROBABILITY_BASE_HORIZONTAL;
    Coords c;
    for (c.x = 0; c.x < g.size; c.x++) {
        for (c.y = 0; c.y < g.size; c.y++) {
            currentHorizontalElementSize = horizontalScan(g, c);
            currentHorizontalInsertionProbability = INSERTION_PROBABILITY_BASE_HORIZONTAL - currentHorizontalElementSize * INSERTION_PROBABILITY_REGRESSION_HORIZONTAL;
            if (currentVerticalElementSize != 0) {
                if (throwDice(currentVerticalInsertionProbability) && testCellLimited(g, c)) {
                    g.grid[c.y][c.x] = INDESTRUCTIBLE;
                    currentVerticalElementSize ++;
                    currentVerticalInsertionProbability -= INSERTION_PROBABILITY_REGRESSION_VERTICAL;
                } else {
                    g.grid[c.y][c.x] = NONE;
                    currentVerticalElementSize = 0;
                    currentVerticalInsertionProbability = INSERTION_PROBABILITY_BASE_VERTICAL;
                }
            } else if (currentHorizontalElementSize != 0) {
                if (throwDice(currentHorizontalInsertionProbability)) {
                    g.grid[c.y][c.x] = INDESTRUCTIBLE;
                } else {
                    g.grid[c.y][c.x] = NONE;
                }
            } else {
                if (throwDice(VERT_HOR_RATIO)) {
                    if (throwDice(INSERTION_PROBABILITY_BASE_HORIZONTAL)) {
                        if (testCellFull(g, c)) {
                            g.grid[c.y][c.x] = INDESTRUCTIBLE;
                        }
                    }
                } else {
                    if (throwDice(INSERTION_PROBABILITY_BASE_VERTICAL)) {
                        if (testCellFull(g, c)) {
                            g.grid[c.y][c.x] = INDESTRUCTIBLE;
                            currentVerticalElementSize ++;
                            currentVerticalInsertionProbability -= INSERTION_PROBABILITY_REGRESSION_VERTICAL;
                        }
                    }
                }
            }
//            if ((currentElementSize < INSERTION_ELEMENT_SIZE_MAX_VERTICAL
//                &&
//                throwDice(currentInsertionProbability))
//            && (currentElementSize != 0
//                &&
//                testCellLimited(grid, c)
//              ||
//                currentElementSize == 0
//                &&
//                testCellFull(grid, c))) {
//                grid.grid[c.y][c.x] = INDESTRUCTIBLE;
//                currentInsertionProbability -= INSERTION_PROBABILITY_REGRESSION_VERTICAL;
//                currentElementSize ++;
//            } else {
//                currentInsertionProbability = INSERTION_PROBABILITY_BASE_VERTICAL;
//                currentElementSize = 0;
//                grid.grid[c.y][c.x] = NONE;
//            }
        }
    }
}

int MapGenerator::random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

bool MapGenerator::throwDice(float probability) {
    float random = (rand() % 100) / 100.;
    if (probability > random)
        return true;
    return false;
}
bool MapGenerator::testCellFull(Grid& grid, Coords coords) {
    bool result = true;
    if (coords.y != 0)
        result = result &&
            grid.grid[coords.y - 1][coords.x - 1] == NONE &&
            grid.grid[coords.y - 1][coords.x] == NONE;
    if (coords.x != 0) {
        if (coords.y != grid.size - 1)
            result = result && grid.grid[coords.y + 1][coords.x - 1] == NONE;
        result = result && grid.grid[coords.y][coords.x - 1] == NONE;
    }
    return result;
}
bool MapGenerator::testCellLimited(Grid& grid, Coords coords) {
    bool result = true;
    if (coords.y != grid.size - 1)
        result = result && grid.grid[coords.y + 1][coords.x - 1] == NONE;
    return result;
}
int MapGenerator::horizontalScan(Grid& grid, Coords coords) {
    int i = 0;
    if (coords.x == 0)
        return 0;
    while(grid.grid[coords.y][coords.x - i - 1] == INDESTRUCTIBLE) {
        i ++;
        if (coords.x - i == 0)
            return i;
    }
    return i;
}