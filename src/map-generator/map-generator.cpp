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

void MapGenerator::generate(Grid& grid) {
    int i, j = 0;
    // On fout tout à 'vide' pour l'instant
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            grid.grid[i][j] = NONE;
        }
    }
    ::srand(time(0));
    int currentElementSize = 0;
    int maxElementSize = 5;
    float baseInsertionProbability = 0.9;
    float currentInsertionProbability = 0.9;
    Coords c;
    c.x = 0;
    c.y = 0;
    grid.grid[4][3] = INDESTRUCTIBLE;
    for (c.x = 0; c.x < SIZE; c.x++) {
        for (c.y = 0; c.y < SIZE; c.y++) {
            if ((currentElementSize < maxElementSize) && throwDice(currentInsertionProbability)) {
                if (currentElementSize != 0) {
                    if(testCellLimited(grid, c)) {
                        grid.grid[c.y][c.x] = INDESTRUCTIBLE;
                        currentInsertionProbability -= 0.2;
                        currentElementSize ++;
                    } else {
                        currentInsertionProbability = baseInsertionProbability;
                        currentElementSize = 0;
                        grid.grid[c.y][c.x] = NONE;
                    }
                } else {
                    if(testCellFull(grid, c)) {
                        grid.grid[c.y][c.x] = INDESTRUCTIBLE;
                        currentInsertionProbability -= 0.2;
                        currentElementSize ++;
                    } else {
                        currentInsertionProbability = baseInsertionProbability;
                        currentElementSize = 0;
                        grid.grid[c.y][c.x] = NONE;
                    }
                }

            } else {
                currentInsertionProbability = baseInsertionProbability;
                currentElementSize = 0;
                grid.grid[c.y][c.x] = NONE;
            }
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
    return (grid.grid[coords.y - 1][coords.x - 1] == NONE &&
            grid.grid[coords.y][coords.x - 1] == NONE &&
            grid.grid[coords.y + 1][coords.x - 1] == NONE &&
            grid.grid[coords.y - 1][coords.x] == NONE);
}
bool MapGenerator::testCellLimited(Grid& grid, Coords coords) {
    return (grid.grid[coords.y + 1][coords.x - 1] == NONE);
}