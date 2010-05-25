/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
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

#include "bombherman.hpp"

#include "game/menu.hpp"
#include "map/map.hpp"

#include "exceptions/display/nosdl-exception.hpp"
#include "exceptions/display/nosvg-exception.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <librsvg/rsvg.h>

namespace bombherman
{
	class Display
	{
	public:
		/// To initialize the video
		static void init();
		/// To stop the video
		static void quit();
		/// To make the video switch between fullscreen and windowed
		static void changeFullscreen();
		/// Display the menu
		/**
		 * @param menu The pointer the menu to display
		 */
		static void displayMenu(Menu *menu);
		/// Set the map
		/**
		 * @param map Pointer to the map to use
		 */
		static void setMap(map::Map *map);
		
		/// Update the barrels
		static void updateBarrels() { updateBarrels(false); }
		/// Update the players
		static void updatePlayers() { updatePlayers(false); }
	
	private:
		static SDL_Surface *svgToSurface(std::string);
		static void initSurfaces();
		static void cleanSurface(SDL_Surface *);
		
		static void updateDisplay(SDL_Surface *, Uint16 = 0, Uint16 = 0, Uint16 = 0, Uint16 = 0);
		
		static void updateBarrels(bool full);
		static void updatePlayers(bool full);
		static void updateMap(bool full);
		
		// To store the SDL display surface
		static SDL_Surface *sDisplay;
		static Uint32 flags;
		static bool isFullscreen;
		static SDL_mutex *mUpdate;
		
		// To store the SDL text color
		static SDL_Color textColor;
		static SDL_Color highlightColor;
		
		// Max Width and Height of the display
		static Uint32 widthMax, heightMax;
		// Width and Height of the display
		static Uint32 width, height;
		
		static SDL_Surface *sBackground;
		
		static map::Map *gMap;
		
		static SDL_Surface *gMapLayer;
		static SDL_Surface *gBarrelsLayer;
		static SDL_Surface *gPlayersLayer;
		
		static SDL_Surface *gPlayers[2][4][2];
		static SDL_Surface *gBomb;
		static SDL_Surface *gExplosion;
		static SDL_Surface *gBarrel;
		static SDL_Surface *gWall;
		static SDL_Surface *gBack;
		
		static int gMapSize;
		static Uint32 gSize;
		static SDL_Rect gBegin;
	};
}

#endif // _DISPLAY_HPP_
