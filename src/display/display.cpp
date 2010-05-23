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

#include "display.hpp"

#ifdef HAVE_SDL

using namespace bombherman;
using namespace bombherman::display;

Display::Display() :
	sDisplay(SDL_GetVideoSurface()),
	textColor(SDL_Color()),
	width(Config::getInt("screenWidth")),
	height(Config::getInt("screenHeight")),
	fontTitle(NULL),
	fontNormal(NULL)
{
	bhout << "Initialize video" << bhendl;
	
	Uint32 wasinit = SDL_WasInit(SDL_INIT_EVERYTHING);
	bool initSuccess(true);
	if ( ! wasinit )
		initSuccess = SDL_Init(SDL_INIT_VIDEO) == 0;
	else if ( !( wasinit & SDL_INIT_VIDEO ) )
		initSuccess = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	
	if ( ! initSuccess )
		throw new exceptions::display::NoSDLException("Can't init Video subsystem of SDL");
	
	Uint32 flags = SDL_HWSURFACE;
	if ( Config::get("fullscreen") == "true" )
		flags |= SDL_FULLSCREEN;
	
	SDL_Rect **modes = SDL_ListModes(0, flags|SDL_FULLSCREEN);
	if ( modes == reinterpret_cast<SDL_Rect**>(0) )
		throw new exceptions::display::NoSDLException("No modes available!");
	
	bool ok;
	
	ok = false;
	if ( modes == reinterpret_cast<SDL_Rect**>(-1) )
	{
		bhout << "All resolutions available." << bhendl;
		if ( ( this->width == 0 ) || ( this->height == 0 ) )
			throw new exceptions::display::NoSDLException("Can't choice the resolution");
	}
	else
	{
		/* Print valid modes */
		bhout << "Available Modes" << bhendl;
		for (int i=0; modes[i]; ++i)
		{
			bhout << modes[i]->w << 'x' << modes[i]->h << bhendl;
			if ( ( this->width == modes[i]->w ) && ( this->height == modes[i]->h ) )
				ok = true;
		}
	}
	
	if ( ! ok )
	{
		this->width = modes[0]->w;
		this->height = modes[0]->h;
		if ( ( flags & SDL_FULLSCREEN ) == 0 )
		{
			bhout << "Not in fullscreen" << bhendl;
			this->width *= 0.9;
			this->height *= 0.9;
		}
	}
	
	SDL_Surface *tmp = SDL_SetVideoMode(this->width, this->height, 32, flags);
	if ( ! tmp )
	{
		bherr << SDL_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible de passer en 640x480 en 16 bpp");
	}
	else
		this->sDisplay = tmp;
	
	SDL_WM_SetCaption(_("Bomb-her-man"), DATADIR"/bomb-her-man.svg");
	
	if ( TTF_Init() == -1 )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'initialiser l'utilisation des polices TrueType");
	}
	else
	{
		fontTitle = TTF_OpenFont(DATADIR"/biolinum.ttf", 26);
		fontNormal = TTF_OpenFont(DATADIR"/biolinum.ttf", 16);
		
		this->textColor.r = 255;
		this->textColor.g = 255;
		this->textColor.b = 255;
		if ( ( ! this->fontTitle ) || ( ! this->fontNormal ) )
		{
			bherr << TTF_GetError() << bhendl;
			throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
		}
	}
}

Display::~Display()
{
	TTF_CloseFont(this->fontTitle);
	TTF_CloseFont(this->fontNormal);
	this->fontTitle = NULL;
	this->fontNormal = NULL;
	TTF_Quit();
	bhout << "Stop video" << bhendl;
	this->sDisplay = NULL;
	if ( SDL_WasInit(SDL_INIT_VIDEO) )
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	bhout << "Video stopped" << bhendl;
}

void
Display::displayMenu(elements::Menu::Type type)
{
	bhout << "Displaying SDL menu" << bhendl;
	std::vector<std::string> menu = elements::Menu::getMenu(type);
	
	SDL_Surface *textSurface, *sMenu = SDL_CreateRGBSurface(SDL_HWSURFACE, this->width, this->height, 32, 0, 0, 0, 0);
	Uint32 dx = TTF_FontLineSkip(fontTitle);
	SDL_Rect r;
	r.x = 0;
	r.y = ( this->width / 2 );
	for ( unsigned int i = 0 ; i < menu.size() ; ++i )
	{
		if ( ! ( textSurface = (TTF_RenderUTF8_Blended(( i == 0 ) ? ( fontTitle ) : ( fontNormal ), menu[i].c_str(), textColor)) ) )
			bherr << "Can't display a line" << bhendl;
		else
		{
			SDL_BlitSurface(textSurface, NULL, this->sDisplay, &r);
			r.x += dx;
			SDL_FreeSurface(textSurface);
		}
	}
	SDL_BlitSurface(sMenu, NULL, this->sDisplay, NULL);
	SDL_FreeSurface(sMenu);
	SDL_UpdateRect(this->sDisplay, 0, 0, this->width, this->height);
}

void
Display::displayMap()
{
}

#endif // HAVE_SDL
