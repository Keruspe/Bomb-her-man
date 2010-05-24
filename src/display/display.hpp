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

#include "display/elements/menu.hpp"
#include "map/map.hpp"

#include "exceptions/display/nosdl-exception.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <librsvg/rsvg.h>

namespace bombherman
{
	namespace display
	{
		class Display
		{
		public:
			/// To initialize the video
			static void init();
			/// To stop the video
			static void quit();
			/// To make the video fullscreen
			static void fullscreen();
			/// To make the video windowed
			static void windowed();
			/// Display the menu
			/**
			 * @param content The content of the menu to display
			 * @param current The current active item of the menu
			 */
			static void displayMenu(std::vector< std::string> content, unsigned int current);
			/// Display the map
			/**
			 * @param map Pointer to the map to display
			 */
			static void setMap(map::Map *map);
		
		private:
			static SDL_Surface *svgToSurface(std::string);
			static void initSurfaces();
			static void cleanSurface(SDL_Surface *);
			
			static void updateDisplay(SDL_Surface *);
			static void newDisplay(Uint32 adds = 0);
			
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
			
			// To store the SDL title font
			static TTF_Font *fontTitle;
			// To store the SDL normal font
			static TTF_Font *fontNormal;
			
			
			static SDL_Surface *gBackground;
			static SDL_Surface *gBarrelsLayer;
			static SDL_Surface *gPlayersLayer;
			
			static SDL_Surface **gPlayers;
			static SDL_Surface *gBomb;
			static SDL_Surface *gExplosion;
			static SDL_Surface *gBarrel;
			static SDL_Surface *gWall;
			static SDL_Surface *gBack;
			
			static Uint32 gSize;
			static SDL_Rect gBegin;
		};
	}
}

#endif // _DISPLAY_HPP_
