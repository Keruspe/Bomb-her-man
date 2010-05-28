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
		
		/// Range getter
		/*
		 * @return The actual range of the player's bombs (int)
		 */
		int getRange();
		
		/// Coords getter
		/*
		 * @return The coords of the player (map::Coords)
		 */
		map::Coords & getCoords();
		
		/// Orientation getter
		/*
		 * @return The orientation of the player (map::Direction)
		 */
		map::Direction & getOrient();
		
		/// Controlling if player can plant a bomb
		/*
		 * @return True if player can plant a bomb
		 */
		bool isAbleToPlantBomb();
		
		/// Score getter
		/*
		 * @return The score of the player
		 */
		int getScore();
		
		/// Id getter
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
		
		/// List of all players
		static std::vector< Player * > players;
		
		/// Get a player by his id
		/*
		 * @param The player's id (int)
		 *
		 * @return A pointer to the player (Player *)
		 */
		static Player * getPlayer(int);
		
		/// Create a new player
		static void newPlayer();
		
		/// Erase all players
		static void clean();
		
		/// Kill the player
		void die();
		
		/// Reinitialize players and start a new game
		static void reInit();
		
		/// Range setter
		/*
		 * @param The new range for the player (int)
		 */
		void setRange(int);
		
		/// Range adder
		/*
		 * @param The range to add to the player, make it negative to remove (int)
		 */
		void addToRange(int);
		
		/// Make the player plant a bomb
		void plantBomb();
		
		/// Increase/decrease the player's score
		/*
		 * @param New points gained (int)
		 */
		void addToScore(int);
		
		/// Plantable bombs number setter
		/*
		 * @param The new number of bombs the player can drop (int)
		 */
		void setPlantableBombs(int);
		
		/// Plantable bombs number adder
		/*
		 * @param The number to add to the number of bombs the player can carry (int)
		 */
		void addToPlantableBombs(int);
		
		/// Reset the player to its default statistics
		void resetToDefaultStats();
		
		/// Coords setter
		/*
		 * @param The new coords of the player (map::Coords)
		 */
		void setCoords(map::Coords &);
		
		/// Which player is out there ?
		/*
		 * @param The coords where to look for any player (map::Coords)
		 *
		 * @return The player if any, or 0 if none (map::Player*)
		 */
		static Player * playerAt(map::Coords &);
		
		/// Is this player still alive ?
		/*
		 * @return True if the player is alive
		 */
		bool isAlive();
		
		/// Murder a poor player
		/*
		 * @param The player to kill (Player *)
		 */
		void kill(Player * killed);

		/// Tells the player that his bomb has exploded (decreases planted bombs counter)
		void bombHasExploded();
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
		static unsigned icyDeadPeople;
	};
};

#endif	/* _PLAYER_HPP_ */

