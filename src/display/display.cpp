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

bool Display::isInit = false;
SDL_Surface *Display::sDisplay = SDL_GetVideoSurface();
Uint32 Display::flags = SDL_SWSURFACE;
SDL_mutex *Display::mUpdate = SDL_CreateMutex();
SDL_Color Display::textColor = SDL_Color();
SDL_Color Display::highlightColor = SDL_Color();
int Display::widthMax = 0;
int Display::heightMax = 0;
int Display::width = 0;
int Display::height = 0;
TTF_Font *Display::fontTitle = NULL;
TTF_Font *Display::fontNormal = NULL;


void
Display::init()
{
	width = Config::getInt("screenWidth");
	height = Config::getInt("screenHeight");
	
	bhout << "Initialize video" << bhendl;
	
	Uint32 wasinit = SDL_WasInit(SDL_INIT_EVERYTHING);
	bool initSuccess(true);
	if ( ! wasinit )
		initSuccess = SDL_Init(SDL_INIT_VIDEO) == 0;
	else if ( !( wasinit & SDL_INIT_VIDEO ) )
		initSuccess = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	
	if ( ! initSuccess )
		throw new exceptions::display::NoSDLException("Can't init Video subsystem of SDL");
	
	SDL_WM_SetCaption(_("Bomb-her-man"), DATADIR"/bomb-her-man.svg");
	
	SDL_Rect **modes = SDL_ListModes(0, flags|SDL_FULLSCREEN);
	if ( modes == reinterpret_cast<SDL_Rect**>(0) )
		throw new exceptions::display::NoSDLException("No modes available!");
	
	bool ok;
	
	ok = false;
	if ( modes == reinterpret_cast<SDL_Rect**>(-1) )
	{
		bhout << "All resolutions available." << bhendl;
		if ( ( width == 0 ) || ( height == 0 ) )
			throw new exceptions::display::NoSDLException("Can't choice the resolution");
		else
			ok = true;
	}
	else
	{
		/* Print valid modes */
		bhout << "Available Modes" << bhendl;
		for ( int i = 0 ; modes[i] ; ++i )
		{
			bhout << modes[i]->w << 'x' << modes[i]->h << bhendl;
			if ( ( width == modes[i]->w ) && ( height == modes[i]->h ) )
				ok = true;
		}
	}
	
	if ( ok )
	{
		heightMax = height;
		widthMax = width;
	}
	else
	{
		widthMax = modes[0]->w;
		heightMax = modes[0]->h;
	}
	
	if ( Config::get("fullscreen") == "true" )
		fullscreen();
	else
		windowed();
	
	if ( TTF_Init() == -1 )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'initialiser l'utilisation des polices TrueType");
	}
	else
	{
		fontTitle = TTF_OpenFont(DATADIR"/biolinum.ttf", (height / 8));
		fontNormal = TTF_OpenFont(DATADIR"/biolinum.ttf", (height / 15));
		
		textColor.r = 255;
		textColor.g = 255;
		textColor.b = 255;
		
		highlightColor.r = 255;
		highlightColor.g = 0;
		highlightColor.b = 0;
		if ( ( ! fontTitle ) || ( ! fontNormal ) )
		{
			bherr << TTF_GetError() << bhendl;
			throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
		}
	}
	
	isInit = true;
}

void
Display::quit()
{
	if ( ! isInit ) return;
	
	//SDL_LockMutex(mUpdate);
	SDL_DestroyMutex(mUpdate);
	
	TTF_CloseFont(fontTitle);
	TTF_CloseFont(fontNormal);
	fontTitle = NULL;
	fontNormal = NULL;
	TTF_Quit();
	bhout << "Stop video" << bhendl;
	sDisplay = NULL;
	if ( SDL_WasInit(SDL_INIT_VIDEO) )
	{
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		bhout << "Video stopped" << bhendl;
	}
	else
		bhout << "Video already stopped" << bhendl;
}

void
Display::newDisplay(Uint32 adds)
{
	SDL_LockMutex(mUpdate);
	SDL_Surface *tmp = SDL_SetVideoMode(width, height, 32, flags|adds);
	if ( ! tmp )
	{
		bherr << SDL_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible de passer en 640x480 en 16 bpp");
	}
	else
	{
		SDL_FreeSurface(sDisplay);
		sDisplay = tmp;
	}
	SDL_UnlockMutex(mUpdate);
}

void
Display::fullscreen()
{
	width = widthMax;
	height = heightMax;
	newDisplay(SDL_FULLSCREEN);
}

void
Display::windowed()
{
	width = widthMax * 0.9;
	height = heightMax * 0.9;
	newDisplay();
}

void
Display::updateDisplay(SDL_Surface *s)
{
	SDL_LockMutex(mUpdate);
	SDL_BlitSurface(s, NULL, sDisplay, NULL);
	SDL_FreeSurface(s);
	SDL_UpdateRect(sDisplay, 0, 0, width, height);
	SDL_UnlockMutex(mUpdate);
}

void
Display::displayMenu(std::vector< std::string> content, unsigned int current)
{
	if ( ! isInit ) init();
	
	bhout << "Displaying menu" << bhendl;
	
	SDL_Surface *textSurface, *sMenu = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	Uint32 dy = ( TTF_FontLineSkip(fontTitle) + ( height / 15 ) ), bx = ( width / 2 );
	SDL_Rect r;
	r.x = 0;
	r.y = ( ( height - ( dy * content.size() ) ) / 2 );
	for ( unsigned int i = 0 ; i < content.size() ; ++i )
	{
		TTF_Font *font = ( i == 0 ) ? ( fontTitle ) : ( fontNormal );
		SDL_Color color = ( i == current ) ? ( highlightColor ) : ( textColor );
		if ( ! ( textSurface = TTF_RenderUTF8_Blended(font, content[i].c_str(), color) ) )
			bherr << "Can't display the line" << content[i] << bhendl;
		else
		{
			int w, h;
			TTF_SizeText(font, content[i].c_str(), &w, &h);
			r.x = bx - ( w / 2 );
			SDL_BlitSurface(textSurface, NULL, sMenu, &r);
			r.y += dy;
			SDL_FreeSurface(textSurface);
		}
	}
	updateDisplay(sMenu);
}

void
Display::displayMap()
{
	if ( ! isInit ) init();
	
	SDL_Surface *sMap = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
	updateDisplay(sMap);
}

#endif // HAVE_SDL
