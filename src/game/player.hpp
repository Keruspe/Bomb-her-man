/* 
 * File:   Player.hpp
 * Author: mogzor
 *
 * Created on May 24, 2010, 11:52 AM
 */

#include "map/map-utils.hpp"

#ifndef _PLAYER_HPP
#define	_PLAYER_HPP

namespace bombherman {

    class Player {
    public:
        Player();
        int getRange();
        map::Coords * getCoords();
        bool isAbleToPlantBomb();
        int getScore();
        int getId();
        void setRange(int);
        void go(map::Direction);
        void plantBomb();
        void addToScore(int);
        void setPlantableBombs(int);
        void addToPlantableBombs(int);
        void resetToDefaultStats();
    private:
        static int maxId;
        int id;
        int range;
        map::Coords * coords;
        int score;
        int plantableBombs;
        int plantedBombs;
    };
};

#endif	/* _PLAYER_HPP */

