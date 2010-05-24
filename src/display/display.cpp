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
#include <librsvg/rsvg-cairo.h>
#include <cairo.h>

using namespace bombherman;
using namespace bombherman::display;

SDL_Surface *Display::sDisplay = NULL;
Uint32 Display::flags = SDL_HWSURFACE;
bool Display::isFullscreen = false;
SDL_mutex *Display::mUpdate = SDL_CreateMutex();

SDL_Color Display::textColor = SDL_Color();
SDL_Color Display::highlightColor = SDL_Color();

Uint32 Display::widthMax = 0;
Uint32 Display::heightMax = 0;
Uint32 Display::width = 0;
Uint32 Display::height = 0;

TTF_Font *Display::fontTitle = NULL;
TTF_Font *Display::fontNormal = NULL;

SDL_Surface *Display::gBackground = NULL;
SDL_Surface *Display::gBarrelsLayer = NULL;
SDL_Surface *Display::gPlayersLayer = NULL;

SDL_Surface **Display::gPlayers = NULL;
SDL_Surface *Display::gBomb = NULL;
SDL_Surface *Display::gExplosion = NULL;
SDL_Surface *Display::gBarrel = NULL;
SDL_Surface *Display::gWall = NULL;
SDL_Surface *Display::gBack = NULL;


Uint32 Display::gSize = 0;

void
Display::init()
{
	width = Config::getInt("screenWidth");
	height = Config::getInt("screenHeight");
	
	bhout << "Initialize video" << bhendl;
	
	Uint32 wasInit = SDL_WasInit(SDL_INIT_EVERYTHING);
	bool initSuccess(true);
	if ( ! wasInit )
		initSuccess = SDL_Init(SDL_INIT_VIDEO) == 0;
	else if ( !( wasInit & SDL_INIT_VIDEO ) )
		initSuccess = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	
	if ( ! initSuccess )
		throw new exceptions::display::NoSDLException("Can't init Video subsystem of SDL");
	
	SDL_WM_SetCaption(_("Bomb-her-man"), "bomb-her-man.svg");
	SDL_ShowCursor(SDL_DISABLE);
	
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
	
	initSurfaces();
}

SDL_Surface *
Display::rsvgToSurface(RsvgHandle *rsvg, double sx = 1, double sy = 0)
{
	if ( sy == 0 ) sy = sx;
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
	Uint32 stride = 4 * gSize;
	void *buffer = calloc(stride * gSize, 1);
	cairo_surface_t *cSurface = cairo_image_surface_create_for_data(static_cast<unsigned char *>(buffer), CAIRO_FORMAT_ARGB32, gSize, gSize, stride);
	cairo_t *cObject = cairo_create(cSurface);
	cairo_scale(cObject, sx, sy);
	rsvg_handle_render_cairo(rsvg, cObject);
	cairo_surface_finish(cSurface);
	SDL_Surface *ret = SDL_CreateRGBSurfaceFrom(buffer, gSize, gSize, 32, stride, rmask, gmask, bmask, amask);
	//free(buffer);
	return ret;
}

void
Display::initSurfaces()
{
	rsvg_init();
	
	RsvgHandle *rsvg = NULL;
	GError **err = NULL;
	RsvgDimensionData dims;
	float s = 0.5;
	
	// Logo
	bhout << "Creating Logo" << bhendl;
	rsvg = rsvg_handle_new_from_file(DATADIR"/bomb-her-man.svg", err);
	rsvg_handle_get_dimensions(rsvg, &dims);
	
	SDL_Surface *icon = rsvgToSurface(rsvg);
	//SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
	icon = NULL;
	g_object_unref(rsvg);
	rsvg = NULL;
	bhout << "Logo created" << bhendl;
	
	// gBomb
	bhout << "Creating gBomb" << bhendl;
	cleanSurface(gBomb);
	bhout << bhendl;
	rsvg = rsvg_handle_new_from_file(DATADIR"/bomb.svg", err);
	bhout << bhendl;
	rsvg_handle_get_dimensions(rsvg, &dims);
	bhout << bhendl;
	//s = dims.width / Display::gSize;
	bhout << bhendl;
	gBomb = rsvgToSurface(rsvg, s);
	bhout << bhendl;
	g_object_unref(rsvg);
	bhout << bhendl;
	rsvg = NULL;
	bhout << "gBomb created" << bhendl;
	
	
	// gWall
	bhout << "Creating gWall" << bhendl;
	cleanSurface(gWall);
	rsvg = rsvg_handle_new_from_file(DATADIR"/wall.svg", err);
	rsvg_handle_get_dimensions(rsvg, &dims);
	gWall = rsvgToSurface(rsvg, s);
	g_object_unref(rsvg);
	rsvg = NULL;
	bhout << "gWall created" << bhendl;
	
	// gBack
	bhout << "Creating gBack" << bhendl;
	cleanSurface(gBack);
	rsvg = rsvg_handle_new_from_file(DATADIR"/back.svg", err);
	rsvg_handle_get_dimensions(rsvg, &dims);
	gBack = rsvgToSurface(rsvg, s);
	g_object_unref(rsvg);
	rsvg = NULL;
	bhout << "gBack created" << bhendl;
	
	rsvg_term();
}

void
Display::cleanSurface(SDL_Surface *surf)
{
	if ( surf )
	{
		SDL_FreeSurface(surf);
		surf = NULL;
	}
}

void
Display::quit()
{
	if ( ! sDisplay ) return;
	
	cleanSurface(gBomb);
	cleanSurface(gWall);
	cleanSurface(gBackground);
	
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
		isFullscreen = ( adds & SDL_FULLSCREEN );
		SDL_FreeSurface(sDisplay);
		sDisplay = tmp;
	}
	SDL_UnlockMutex(mUpdate);
}

void
Display::fullscreen()
{
	if ( ( sDisplay ) && ( isFullscreen ) ) return;
	width = widthMax;
	height = heightMax;
	newDisplay(SDL_FULLSCREEN);
}

void
Display::windowed()
{
	if ( ( sDisplay ) && ( ! isFullscreen ) ) return;
	width = widthMax * 0.9;
	height = heightMax * 0.9;
	newDisplay();
}

void
Display::updateDisplay(SDL_Surface *s)
{
	SDL_LockMutex(mUpdate);
	SDL_BlitSurface(s, NULL, sDisplay, NULL);
	SDL_UpdateRect(sDisplay, 0, 0, width, height);
	SDL_UnlockMutex(mUpdate);
}

void
Display::displayMenu(std::vector< std::string> content, unsigned int current)
{
	if ( ! sDisplay ) init();
	
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
	SDL_FreeSurface(sMenu);
}

void
Display::displayMap(map::Map *map)
{
	if ( ! sDisplay ) init();
	
	SDL_Rect r;
	int size = Config::getInt("mapSize");
	Uint32 bx = gSize * size;
	
	cleanSurface(gBackground);
	gBackground = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	if ( ! gWall ) initSurfaces();
	map::Coords coords;
	r.y = 0;
	for(coords.y = 0 ; coords.y < size ; ++coords.y)
	{
		r.x = bx;
		for(coords.x = 0 ; coords.x < size ; ++coords.x)
		{
			if ( map->get(coords) == 'x' )
				SDL_BlitSurface(gWall, NULL, gBackground, &r);
			else
				SDL_BlitSurface(gBack, NULL, gBackground, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	SDL_Surface *test = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(gBack, NULL, test, NULL);
	updateDisplay(test);
	SDL_FreeSurface(test);
}
