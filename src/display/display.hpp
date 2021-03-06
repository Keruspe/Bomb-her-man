/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
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

#ifndef _DISPLAY_HPP_
#define _DISPLAY_HPP_

#include <SDL.h>
#include <SDL_ttf.h>
#include <librsvg/rsvg.h>
#include <map>

#include "bombherman.hpp"
#include "game/menu.hpp"
#include "map/map.hpp"
#include "exceptions/display/nosdl-exception.hpp"
#include "exceptions/display/nosvg-exception.hpp"

namespace bombherman
{
	#define NB_BONUSES map::LASTBONUS - map::FIRSTBONUS + 1
	
	/// Class used to manage all the graphical things
	class Display
	{
	public:
		/// To initialize the video
		static void init();
		
		/// Inform the display we quit the game (it can free some surfaces)
		static void quitGame();
		
		/// To stop the video
		static void quit();
		
		/// Set the window mode (window/fullscreen)
		static void setMode();
		/// Set the window size (width/height)
		/**
		 * @param mode The index of the mode in the vector
		 */
		static void setMode(Uint8 mode);
		
		/// Display the menu
		/**
		 * @param menu The pointer the menu to display
		 */
		static void displayMenu(Menu * menu);
		
		/// Get the video modes
		/**
		 * @return A vector of pair width/height
		 */
		static std::vector< std::pair< Uint16, Uint16 > > getModes();
		
		/// Update the map
		static void updateMap();
		
		/// Move a player
		/**
		 * @param player The player who moves (Player *)
		 * @param moveResult The result of the move (map::MoveResult)
		 */
		static void movePlayer(Player * player, map::MoveResult moveResult);
		
		/// Plant a bomb
		/**
		 * @param player (Pointer to the) Player who plant the bomb
		 */
		static void plantBomb(Player * player);
		
		/// Show the bomb explosing
		/**
		 * @param coords Where the bomb was placed
		 * @param cells Which cells (a vector of Coords) the bomb destroyed
		 */
		static void explode(map::Coords coords, std::vector< map::Coords > cells);
		
		/// Show the final scores screen
		static void displayScores();
	private:
		static SDL_Surface * svgToSurface(std::string, Uint32 = gSize, Uint32 = gSize);
		static void initSurfaces();
		static void cleanSurface(SDL_Surface * &);
		static void updateDisplay(SDL_Surface * s, SDL_Rect z) { updateDisplay(s, z.x, z.y, z.w, z.h); }
		static void updateDisplay(SDL_Surface *, Uint16 = 0, Uint16 = 0, Uint16 = 0, Uint16 = 0);
		
		// Update the scores "panel"
		static void updateScores(bool = false);
		
		// Update the barrels
		static void updateBarrels();
		
		// Update the players
		static void updatePlayers();
		
		// To store the SDL display surface
		static SDL_Surface * sDisplay;
		static Uint32 flags;
		static bool isFullscreen;
		static SDL_mutex * mUpdate;
		
		// To store the SDL text color
		static SDL_Color textColor;
		static SDL_Color highlightColor;
		static SDL_Color scoreColor;
		
		static int widthMax, heightMax;
		static int width, height;
		static std::vector< std::pair< Uint16, Uint16 > > displayModes;
		
		static std::map< SDL_Surface *, unsigned char * > buffers;
		static SDL_Surface * sBackground;
		
		static SDL_Surface * gScoresLayer;
		static SDL_Surface * gMapLayer;
		static SDL_Surface * gBarrelsLayer;
		static SDL_Surface * gPlayersLayer;
		
		static std::vector< std::vector< std::vector< SDL_Surface * > > > gPlayers;
		static SDL_Surface * gBonuses[NB_BONUSES];
		static SDL_Surface * gBomb;
		static SDL_Surface * gExplosion;
		static SDL_Surface * gBarrel;
		static SDL_Surface * gTomb[2];
		static SDL_Surface * gFloor;
		
		static Uint32 gMapSize;
		static Uint16 gSize;
		static SDL_Rect gZone;
	};
}

#endif // _DISPLAY_HPP_

