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
		virtual ~Player();
		int getRange();
		map::Coords & getCoords();
		map::Direction & getOrient();
		bool isAbleToPlantBomb();
		int getScore();
		int getId();
		bool go(map::Direction &);
		static std::vector< Player * > & getPlayers();
		static Player * getPlayer(int);
		static void newPlayer();
		static void clean();
		void setRange(int);
		void addToRange(int);
		void plantBomb();
		void addToScore(int);
		void setPlantableBombs(int);
		void addToPlantableBombs(int);
		void resetToDefaultStats();
		void setCoords(map::Coords &);
		static Player * playerAt(map::Coords *);
	private:
		Player();
		int plantableBombs;
		int range;
		int plantedBombs;
		int score;
		int id;
		map::Coords coords;
		map::Direction orient;
		static std::vector< Player * > players;
	};
};

#endif	/* _PLAYER_HPP_ */

