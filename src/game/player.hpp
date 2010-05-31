/* 
 * File:   Player.hpp
 * Author: mogzor
 *
 * Created on May 24, 2010, 11:52 AM
 */

#ifndef _PLAYER_HPP_
#define	_PLAYER_HPP_

#include "map/map.hpp"

namespace bombherman
{
	/// A player
	class Player
	{
	public:
		/// Destructor
		virtual ~Player() {}
		
		/// Range getter
		/**
		 * @return The actual range of the player's bombs (int)
		 */
		int getRange() { return this->range; }
		
		/// Coords getter
		/**
		 * @return The coords of the player (map::Coords)
		 */
		map::Coords & getCoords() { return this->coords; }
		
		/// Orientation getter
		/**
		 * @return The orientation of the player (map::Direction)
		 */
		map::Direction & getOrient() { return this->orient; }
		
		/// Controlling if player can plant a bomb
		/**
		 * @return True if player can plant a bomb (bool)
		 */
		bool isAbleToPlantBomb() { return (this->plantableBombs > this->plantedBombs); }
		
		/// Score getter
		/**
		 * @return The score of the player (int)
		 */
		int getScore() { return this->score; }
		
		/// Id getter
		/**
		 * @return The id of the player (int)
		 */
		int getId() { return this->id; }
		
		/// To move
		/**
		 * @param direction The direction where player wants to go (map::Direction)
		 * 
		 * @return What did happen during the move (map::MoveResult)
		 */
		map::MoveResult go(map::Direction & direction);
		
		/// List of all players
		static std::vector< Player * > players;
		
		/// Get a player by his id
		/**
		 * @param id The player's id (int)
		 *
		 * @return A pointer to the player (Player *)
		 */
		static Player * getPlayer(int id);
		
		/// Create a new player
		static void newPlayer();
		
		/// Erase all players
		static void clean();
		
		/// Kill the player
		/**
		 * @return True if the game is over
		 */
		bool die();
		
		/// Reinitialize players and start a new game
		/**
		 * @param dummy A void pointer to make SDL_Thread happy (void *)
		 *
		 * @return 0 to make SDL_Thread happy
		 */
		static int reInit(void * dummy);
		
		/// Range setter
		/**
		 * @param range The new range for the player (int)
		 */
		void setRange(int range);
		
		/// Range adder
		/**
		 * @param range The range to add to the player, make it negative to remove (int)
		 */
		void addToRange(int range) { this->setRange(this->range + range); }
		
		/// Make the player plant a bomb
		void plantBomb();
		
		/// Increase/decrease the player's score
		/**
		 * @param score New points gained (int)
		 */
		void addToScore(int score);
		
		/// Plantable bombs number setter
		/**
		 * @param plantableBombs The new number of bombs the player can drop (int)
		 */
		void setPlantableBombs(int plantableBombs);
		
		/// Plantable bombs number adder
		/**
		 * @param plantableBombs The number to add to the number of bombs the player can carry (int)
		 */
		void addToPlantableBombs(int plantableBombs) { this->setPlantableBombs(this->plantableBombs + plantableBombs); }
		
		/// Reset the player to its default statistics
		void resetToDefaultStats();
		
		/// Coords setter
		/**
		 * @param c The new coords of the player (map::Coords)
		 */
		void setCoords(map::Coords & c) { this->coords = c; }
		
		/// Which player is out there ?
		/**
		 * @param c The coords where to look for any player (map::Coords)
		 *
		 * @return The player if any, or 0 if none (map::Player*)
		 */
		static Player * playerAt(map::Coords & c);
		
		/// Is this player still alive ?
		/**
		 * @return True if the player is alive
		 */
		bool isAlive() { return this->alive; }
		
		/// Murder a poor player
		/**
		 * @param killed The player to kill (Player *)
		 *
		 * @return True if the game is over
		 */
		bool kill(Player * killed);

		/// Tells the player that his bomb has exploded (decreases planted bombs counter)
		void bombHasExploded() { --this->plantedBombs; }
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

