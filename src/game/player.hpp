/* 
 * File:   Player.hpp
 * Author: mogzor
 *
 * Created on May 24, 2010, 11:52 AM
 */

#include "map/map-utils.hpp"

#ifndef _PLAYER_HPP_
#define	_PLAYER_HPP_

namespace bombherman
{
	class Player
	{
	public:
		Player(int, map::Coords &);
		int getRange();
		map::Coords getCoords();
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
		int plantableBombs;
		int range;
		int plantedBombs;
		static int maxId;
		int score;
		int id;
		map::Coords coords;
	};
};

#endif	/* _PLAYER_HPP_ */

