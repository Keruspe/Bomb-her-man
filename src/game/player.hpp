/* 
 * File:   Player.hpp
 * Author: mogzor
 *
 * Created on May 24, 2010, 11:52 AM
 */

#include "map/map-utils.hpp"

#ifndef _PLAYER_HPP
#define	_PLAYER_HPP

namespace bombherman
{
    class Player
    {
        public:
            Player();
            int getRange();
            map::Coords * getCoords();
            bool isAbleToPlantBomb();
            int getScore();
            void setRange(int);
            void goLeft();
            void goRight();
            void goTop();
            void goBottom();
            void plantBomb();
            void addToScore(int);
            void setPlantableBombs(int);
            void addToPlantableBombs(int);
            void resetToDefaultStats();
        private:
            int range;
            map::Coords * coords;
            int score;
            int plantableBombs;
            int plantedBombs;
    };
};

#endif	/* _PLAYER_HPP */

