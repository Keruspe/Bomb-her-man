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

#include "bombherman.hpp"
#include "sdl.hpp"

using namespace bombherman;
using namespace bombherman::display;
using namespace bombherman::display::backends;

SDL::SDL()
{
	Logger::putLine("Initialize video");
	sDisplay = &SDLmm::Display::GetDisplay();
	sDisplay->Init();
	bool ok = sDisplay->SetVideoMode(640, 480, 16, SDL_SWSURFACE);
	if ( ! ok )
	{
		Logger::putLine("Impossible de passer en 640x480 en 16 bpp:", true);
		Logger::putLine(SDLmm::GetError(), true);
	//	throw exceptions::NoSDL;
	}
	sDisplay->SetCaption(_("Bomb-her-man"), "bomb-her-man.svg");
}

SDL::~SDL()
{
	Logger::putLine("Stop video");
	sDisplay->Quit();
	Logger::putLine("Video stopped");
}

bool
SDL::displayMenu(elements::MenuType type)
{
	Logger::putLine("Displaying SDL menu");
	std::vector<std::string> menu = elements::Menu::getMenu(type);
	
	SDLmm::SRect rect(10, 10, 100, 100);
	
	sDisplay->FillRect(rect, 0x009090);
	
	sDisplay->UpdateRect(rect);
}

bool
SDL::displayMap()
{
}
