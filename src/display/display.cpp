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
#include <sstream>
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

SDL_Surface *Display::sBackground = NULL;

map::Map *Display::gMap = NULL;

SDL_Surface *Display::gMapLayer = NULL;
SDL_Surface *Display::gBarrelsLayer = NULL;
SDL_Surface *Display::gPlayersLayer = NULL;

SDL_Surface *Display::gPlayers[2][4][2] =
{
	{
		{NULL, NULL},
		{NULL, NULL},
		{NULL, NULL},
		{NULL, NULL}
	},
	{
		{NULL, NULL},
		{NULL, NULL},
		{NULL, NULL},
		{NULL, NULL}
	}
};
SDL_Surface *Display::gBomb = NULL;
SDL_Surface *Display::gExplosion = NULL;
SDL_Surface *Display::gBarrel = NULL;
SDL_Surface *Display::gWall = NULL;
SDL_Surface *Display::gBack = NULL;


int Display::gMapSize = 0;
Uint32 Display::gSize = 0;
SDL_Rect Display::gBegin;

void
Display::init()
{
	width = Config::getInt("screenWidth");
	height = Config::getInt("screenHeight");
	gMapSize = Config::getInt("mapSize");
	
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
}

SDL_Surface *
Display::svgToSurface(std::string file)
{
	GError **err = NULL;
	RsvgHandle *rsvg = rsvg_handle_new_from_file(file.c_str(), err);
	if ( err )
		throw new exceptions::display::NoSVGException("Can't read the file");
	RsvgDimensionData dims;
	rsvg_handle_get_dimensions(rsvg, &dims);
	
	Uint32 stride = 4 * gSize;
	void *buffer = calloc(stride * gSize, 1);
	cairo_surface_t *cSurface = cairo_image_surface_create_for_data(static_cast<unsigned char *>(buffer), CAIRO_FORMAT_ARGB32, gSize, gSize, stride);
	cairo_t *cObject = cairo_create(cSurface);
	
	double s = static_cast<double>(Display::gSize) / static_cast<double>(dims.width);
	cairo_scale(cObject, s, s);
	bhout << "Scale to " << s << bhendl;
	
	rsvg_handle_render_cairo(rsvg, cObject);
	g_object_unref(rsvg);
	rsvg = NULL;
	
	cairo_surface_finish(cSurface);
	
	SDL_Surface *ret = SDL_CreateRGBSurfaceFrom(buffer, gSize, gSize, 32, stride, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	//free(buffer);
	
	return ret;
}

void
Display::initSurfaces()
{
	rsvg_init();
	
	
	// Logo
	bhout << "Creating Logo" << bhendl;
	SDL_Surface *icon = svgToSurface(DATADIR"/bomb-her-man.svg");
	//SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
	icon = NULL;
	bhout << "Logo created" << bhendl;
	
	
	// gBomb
	bhout << "Creating gBomb" << bhendl;
	cleanSurface(gBomb);
	gBomb = svgToSurface(DATADIR"/bomb.svg");
	bhout << "gBomb created" << bhendl;
	
	
	// gWall
	bhout << "Creating gWall" << bhendl;
	cleanSurface(gWall);
	gWall = svgToSurface(DATADIR"/wall.svg");
	bhout << "gWall created" << bhendl;
	
	// gBack
	bhout << "Creating gBack" << bhendl;
	cleanSurface(gBack);
	gBack = svgToSurface(DATADIR"/back.svg");
	bhout << "gBack created" << bhendl;
	
	// gBarrel
	bhout << "Creating gBarrel" << bhendl;
	cleanSurface(gBarrel);
	gBarrel = svgToSurface(DATADIR"/barrel.svg");
	bhout << "gBarrel created" << bhendl;
	
	
	/*
	 * Players
	 */
	char max = Config::getInt("maxPlayers") + '1';
	for ( unsigned char p = '1' ; p < max ; ++p )
	{
		bhout << "Make Player " << p << bhendl;
		for ( unsigned char i = '0' ; i < '4' ; ++i )
		{
			bhout << "	Make Face " << i << bhendl;
			for ( unsigned char j = '0' ; j < '2' ; ++j )
			{
				bhout << "		Make Image " << j << bhendl;
				cleanSurface(gPlayers[p-'0'-1][i-'0'][j-'0']);
				
				std::ostringstream f;
				f << DATADIR << "/players/" << p << '/' << i << '/' << j << ".svg";
				bhout << "File " << f.str() << bhendl; 
				gPlayers[p-'0'-1][i-'0'][j-'0'] = svgToSurface(f.str());
				
				//gPlayers[p-'0'-1][i-'0'-1][j-'0'-1] = svgToSurface(DATADIR"/her.svg");
			}
		}
	}
	
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
	cleanSurface(gMapLayer);
	
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
	if ( height < width )
	{
		gSize = height / gMapSize;
		gBegin.x = width - height;
		gBegin.y = 0;
	}
	else
	{
		gSize = width / gMapSize;
		gBegin.x = 0;
		gBegin.y = height - width;
	}
	cleanSurface(sBackground);
	sBackground = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(sBackground, NULL, 0x00444444);
	initSurfaces();
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
	SDL_BlitSurface(sBackground, NULL, sMenu, NULL);
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
Display::setMap(map::Map *map)
{
	if ( ! sDisplay ) init();
	
	gMap = map;
	
	updateMap();
}

void
Display::updateMap()
{
	SDL_Rect r;
	cleanSurface(gMapLayer);
	gMapLayer = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(sBackground, NULL, gMapLayer, NULL);
	if ( ! gWall ) initSurfaces();
	map::Coords coords;
	r.y = gBegin.y;
	for(coords.y = 0 ; coords.y < gMapSize ; ++coords.y)
	{
		r.x = gBegin.x;
		for(coords.x = 0 ; coords.x < gMapSize ; ++coords.x)
		{
			if ( gMap->get(coords) == 'x' )
				SDL_BlitSurface(gWall, NULL, gMapLayer, &r);
			else
				SDL_BlitSurface(gBack, NULL, gMapLayer, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	updateBarrels();
}

void
Display::updateBarrels()
{
	SDL_Rect r;
	cleanSurface(gBarrelsLayer);
	gBarrelsLayer = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(gMapLayer, NULL, gBarrelsLayer, NULL);
	if ( ! gWall ) initSurfaces();
	map::Coords coords;
	r.y = gBegin.y;
	for(coords.y = 0 ; coords.y < gMapSize ; ++coords.y)
	{
		r.x = gBegin.x;
		for(coords.x = 0 ; coords.x < gMapSize ; ++coords.x)
		{
			if (  gMap->get(coords) == 't' )
				SDL_BlitSurface(gBarrel, NULL, gBarrelsLayer, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	updatePlayers();
}

void
Display::updatePlayers()
{
	SDL_Rect r;
	cleanSurface(gPlayersLayer);
	gPlayersLayer = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(gBarrelsLayer, NULL, gPlayersLayer, NULL);
	if ( ! gPlayers[0][0][0] ) initSurfaces();
	map::Coords coords;
	r.y = gBegin.y;
	for(coords.y = 0 ; coords.y < gMapSize ; ++coords.y)
	{
		r.x = gBegin.x;
		for(coords.x = 0 ; coords.x < gMapSize ; ++coords.x)
		{
			char c = gMap->get(coords);
			if ( ( c == '1' ) || ( c == '2' ) )
				SDL_BlitSurface(gPlayers[c-'0'-1][0][0], NULL, gPlayersLayer, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	updateDisplay(gPlayersLayer);
}
