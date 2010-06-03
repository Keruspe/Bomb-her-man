/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

#ifndef _PLAYER_HPP_
#define	_PLAYER_HPP_

#include <SDL_mutex.h>

#include "map/map.hpp"

namespace bombherman
{
	/// A player
	class Player
	{
	public:
		/// Destructor
		virtual ~Player() { SDL_DestroyMutex(move_mutex); }
		
		/// Range getter
		/**
		 * @return The actual range of the player's bombs (Uint32)
		 */
		Uint32 getRange() { return this->range; }
		
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
		 */
		void go(map::Direction direction);
		
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
		 * @param range The new range for the player (Uint32)
		 */
		void setRange(Uint32 range);
		
		/// Range adder
		/**
		 * @param range The range to add to the player, make it negative to remove (Uint32)
		 */
		void addToRange(Uint32 range) { this->setRange(this->range + range); }
		
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
	protected:
		/// Private constructor, please call static newPlayer instead
		Player();
		
		/// Number of bombs the player can plant (int)
		int plantableBombs;
		
		/// Current range (Uint32)
		Uint32 range;
		
		/// Number of planted bombs (int)
		int plantedBombs;
		
		/// Current score (int)
		int score;
		
		/// Id of the player (int)
		int id;
		
		/// Is the player alive ? (bool)
		bool alive;
		
		/// Coords of the player (map::Coords)
		map::Coords coords;
		
		/// Orientation of the player (map::Direction)
		map::Direction orient;
		
		/// Number of dead people (unsigned)
		static unsigned icyDeadPeople;
		
		/// Mutex for handling moves
		SDL_mutex * move_mutex;
		/// Current number of moves move
		Uint8 currentMoves;
	};
};

#endif	/* _PLAYER_HPP_ */

