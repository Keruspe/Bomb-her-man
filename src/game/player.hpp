/* 
 * File:   Player.hpp
 * Author: mogzor
 *
 * Created on May 24, 2010, 11:52 AM
 */

#ifndef _PLAYER_HPP_
#define	_PLAYER_HPP_

#include "map/map-utils.hpp"

namespace bombherman
{
	class Player
	{
	public:
		virtual ~Player();
		
		/// range getter
		/*
		 * @return The actual range of the player's bombs (int)
		 */
		int getRange();
		
		/// coords getter
		/*
		 * @return The coords of the player (map::Coords)
		 */
		map::Coords & getCoords();
		
		/// orientation getter
		/*
		 * @return The orientation of the player (map::Direction)
		 */
		map::Direction & getOrient();
		
		/// Controlling if player can plant a bomb
		/*
		 * @return True if player can plant a bomb
		 */
		bool isAbleToPlantBomb();
		
		/// score getter
		/*
		 * @return The score of the player
		 */
		int getScore();
		
		/// id getter
		/*
		 * @return The id of the player
		 */
		int getId();
		
		/// To move
		/*
		 * @param The direction where player wants to go (map::Direction)
		 * 
		 * @return What did happen during the move (map::MoveResult)
		 */
		map::MoveResult go(map::Direction &);
		
		static std::vector< Player * > & getPlayers();
		static Player * getPlayer(int);
		static void newPlayer();
		static void clean();
		void die();
		static void reInit();
		void setRange(int);
		void addToRange(int);
		void plantBomb();
		void addToScore(int);
		void setPlantableBombs(int);
		void addToPlantableBombs(int);
		void resetToDefaultStats();
		void setCoords(map::Coords &);
		static Player * playerAt(map::Coords &);
		bool isAlive();
		void kill(Player *killed);
	private:
		/// Private constructor, please call static newPlayer instead
		Player();
		int plantableBombs;
		int range;
		int plantedBombs;
		int score;
		int id;
		bool alive;
		map::Coords coords;
		map::Direction orient;
		static std::vector< Player * > players;
		static unsigned icyDeadPeople;
	};
};

#endif	/* _PLAYER_HPP_ */

