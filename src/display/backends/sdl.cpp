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

#include "sdl.hpp"

#ifdef HAVE_SDLMM

using namespace bombherman;
using namespace bombherman::display;
using namespace bombherman::display::backends;

SDL::SDL() : sDisplay(&SDLmm::Display::GetDisplay()),
		textColor(SDL_Color()),
		width(Config::getInt("screenWidth")),
		height(Config::getInt("screenHeight")),
		fontTitle(TTF_OpenFont(DATADIR"/biolinum.ttf", 26)),
		fontNormal(TTF_OpenFont(DATADIR"/biolinum.ttf", 16))
{
	bhout << "Initialize video" << bhendl;
	
	sDisplay->Init();
	
	Uint32 flags = SDL_SWSURFACE;
	if ( Config::get("fullscreen") == "true" )
		flags |= SDL_FULLSCREEN;
	
	SDL_Rect **modes = SDLmm::Display::ListModes(0, flags|SDL_FULLSCREEN);
	if ( modes == reinterpret_cast<SDL_Rect**>(0) )
		throw new exceptions::display::NoSDLException("No modes available!");
	
	bool ok;
	
	ok = false;
	if ( modes == reinterpret_cast<SDL_Rect**>(-1) )
	{
		bhout << "All resolutions available." << bhendl;
		if ( ( width == 0 ) || ( height == 0 ) )
			throw new exceptions::display::NoSDLException("Can't choice the resolution");
	}
	else
	{
		/* Print valid modes */
		bhout << "Available Modes" << bhendl;
		for (int i=0; modes[i]; ++i)
		{
			bhout << modes[i]->w << 'x' << modes[i]->h << bhendl;
			if ( ( width == modes[i]->w ) && ( height == modes[i]->h ) )
				ok = true;
		}
	}
	
	if ( ! ok )
	{
		width = modes[0]->w;
		height = modes[0]->h;
		if ( (flags&SDL_FULLSCREEN) == 0 )
		{
			bhout << "Not in fullscreen" << bhendl;
			width *= 0.9;
			height *= 0.9;
		}
	}
	
	ok = true;
	ok = sDisplay->SetVideoMode(width, height, 32, flags);
	if ( ! ok )
	{
		bherr << SDLmm::GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible de passer en 640x480 en 16 bpp");
	}
	sDisplay->SetCaption(_("Bomb-her-man"), DATADIR"/bomb-her-man.svg");
	
	if ( TTF_Init() == -1 )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'initialiser l'utilisation des polices TrueType");
	}
	else
	{
		textColor.r = 255;
		textColor.g = 255;
		textColor.b = 255;
		if ( ( ! fontTitle ) || ( ! fontNormal ) )
		{
			bherr << TTF_GetError() << bhendl;
			throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
		}
	}
}

SDL::~SDL()
{
	TTF_CloseFont(fontTitle);
	TTF_CloseFont(fontNormal);
	fontTitle = NULL;
	fontNormal = NULL;
	TTF_Quit();
	bhout << "Stop video" << bhendl;
	sDisplay->Quit();
	bhout << "Video stopped" << bhendl;
}

void
SDL::displayMenu(elements::Menu::Type type)
{
	bhout << "Displaying SDL menu" << bhendl;
	std::vector<std::string> menu = elements::Menu::getMenu(type);
	
	SDLmm::Surface sMenu = SDLmm::Surface::CreateSurface(*sDisplay);
	SDLmm::SPoint diff(TTF_FontLineSkip(fontTitle), 0), point(0, sDisplay->w()/2);
	
	SDLmm::Surface *textSurface;
	for ( unsigned int i = 0 ; i < menu.size() ; ++i )
	{
		if( ! ( textSurface = new SDLmm::Surface(TTF_RenderUTF8_Blended(( i == 0 ) ? ( fontTitle ) : ( fontNormal ), menu[i].c_str(), textColor)) ) )
			bherr << "Can't display a line" << bhendl;
		else
		{
			sDisplay->Blit(*textSurface, point);
			point += diff;
			delete textSurface;
		}
	}
	sDisplay->Blit(sMenu);
	sDisplay->Update();
}

void
SDL::displayMap()
{
}

#endif // HAVE_SDLMM
