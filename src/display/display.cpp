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
#include "game/player.hpp"
#include <sstream>
#include <librsvg/rsvg-cairo.h>
#include <cairo.h>

#define FONT_FILE "biolinum.ttf"

using namespace bombherman;

SDL_Surface *Display::sDisplay = NULL;
Uint32 Display::flags = SDL_HWSURFACE;
bool Display::isFullscreen = false;
SDL_mutex *Display::mUpdate = SDL_CreateMutex();

SDL_Color Display::textColor = {255, 255, 255, 0};
SDL_Color Display::highlightColor = {255, 0, 0, 0};

int Display::widthMax = 0;
int Display::heightMax = 0;
int Display::width = 0;
int Display::height = 0;

std::map<SDL_Surface *, void *> Display::buffers;

SDL_Surface *Display::sBackground = NULL;

SDL_Surface *Display::gScoresLayer = NULL;
SDL_Surface *Display::gMapLayer = NULL;
SDL_Surface *Display::gBarrelsLayer = NULL;
SDL_Surface *Display::gPlayersLayer = NULL;

std::vector< std::vector< std::vector<SDL_Surface *> > > Display::gPlayers;
SDL_Surface *Display::gBomb = NULL;
SDL_Surface *Display::gExplosion = NULL;
SDL_Surface *Display::gBarrel = NULL;
SDL_Surface *Display::gWall = NULL;
SDL_Surface *Display::gBack = NULL;


int Display::gMapSize = 0;
Uint16 Display::gSize = 0;
SDL_Rect Display::gZone;

void
Display::init()
{
	width = Config::getInt("screenWidth");
	height = Config::getInt("screenHeight");
	gMapSize = Config::getInt("mapSize");
	
	unsigned int max = Config::getInt("maxPlayers");
	for ( unsigned int p = 0 ; p < max ; ++p )
	{
		std::vector< std::vector<SDL_Surface *> > k;
		for ( unsigned int i = 0 ; i < 4 ; ++i )
		{
			std::vector<SDL_Surface *> l;
			for ( unsigned int j = 0 ; j < ANIM_IMAGES ; ++j )
				l.push_back(NULL);
			k.push_back(l);
		}
		gPlayers.push_back(k);
	}
	
	Uint32 wasInit = SDL_WasInit(SDL_INIT_EVERYTHING);
	bool initSuccess(true);
	if ( ! wasInit )
		initSuccess = SDL_Init(SDL_INIT_VIDEO) == 0;
	else if ( !( wasInit & SDL_INIT_VIDEO ) )
		initSuccess = SDL_InitSubSystem(SDL_INIT_VIDEO) == 0;
	
	if ( ! initSuccess )
		throw exceptions::display::NoSDLException("Can't init Video subsystem of SDL");
	
	SDL_WM_SetCaption(_("Bomb-her-man"), "bomb-her-man.svg");
	SDL_ShowCursor(SDL_DISABLE);
	
	SDL_Rect **modes = SDL_ListModes(0, flags|SDL_FULLSCREEN);
	if ( modes == reinterpret_cast<SDL_Rect**>(0) )
		throw exceptions::display::NoSDLException("No modes available!");
	
	bool ok = false;
	if ( modes == reinterpret_cast<SDL_Rect**>(-1) )
	{
		if ( ( width == 0 ) || ( height == 0 ) )
			throw exceptions::display::NoSDLException("Can't choice the resolution");
	}
	else
	{
		for ( int i = 0 ; modes[i] ; ++i )
		{
			if ( ( width == modes[i]->w ) && ( height == modes[i]->h ) )
			{
				ok = true;
				break;
			}
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
	
	if ( TTF_Init() == -1 )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'initialiser l'utilisation des polices TrueType");
	}
	
	rsvg_init();
	
	
	changeFullscreen();
}

SDL_Surface *
Display::svgToSurface(std::string file, Uint32 targetWidth, Uint32 targetHeight)
{
	GError **err = NULL;
	
	RsvgHandle *rsvg = rsvg_handle_new_from_file(file.c_str(), err);
	
	if ( err )
		throw exceptions::display::NoSVGException("Can't read the file");
	
	RsvgDimensionData dims;
	
	rsvg_handle_get_dimensions(rsvg, &dims);
	
	double sW = static_cast<double>(targetWidth) / static_cast<double>(dims.width);
	double sH = static_cast<double>(targetHeight) / static_cast<double>(dims.height);
	
	Uint32 stride = 4 * targetWidth;
	void *buffer = calloc(stride * targetHeight, 1);
	
	cairo_surface_t *cSurface = cairo_image_surface_create_for_data(static_cast<unsigned char *>(buffer), CAIRO_FORMAT_ARGB32, targetWidth, targetHeight, stride);
	cairo_t *cObject = cairo_create(cSurface);
	
	cairo_scale(cObject, sW, sH);
	
	rsvg_handle_render_cairo(rsvg, cObject);
	
	cairo_surface_finish(cSurface);
	
	SDL_Surface *ret = SDL_CreateRGBSurfaceFrom(buffer, targetWidth, targetHeight, 32, stride, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	
	g_object_unref(rsvg);
	cairo_destroy(cObject);
	cairo_surface_destroy(cSurface);
	buffers[ret] = buffer;
	
	return ret;
}

void
Display::initSurfaces()
{
	if ( ! sDisplay ) init();
	
	// Logo
	SDL_Surface *icon = svgToSurface(DATADIR"/bomb-her-man.svg");
	//SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
	icon = NULL;
	
	
	// sBackground
	cleanSurface(sBackground);
	sBackground = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(sBackground, NULL, 0x00444444);
	
	
	// gBomb
	cleanSurface(gBomb);
	gBomb = svgToSurface(DATADIR"/bomb.svg");
	
	
	// gWall
	cleanSurface(gWall);
	gWall = svgToSurface(DATADIR"/wall.svg");
	
	// gBack
	cleanSurface(gBack);
	gBack = svgToSurface(DATADIR"/back.svg");
	
	// gBarrel
	cleanSurface(gBarrel);
	gBarrel = svgToSurface(DATADIR"/barrel.svg");
	
	
	/*
	 * Players
	 */
	unsigned int max = Config::getInt("maxPlayers");
	for ( unsigned int p = 0 ; p < max ; ++p )
	{
		for ( unsigned int i = 0 ; i < 4 ; ++i )
		{
			for ( unsigned int j = 0 ; j < ANIM_IMAGES ; ++j )
			{
				cleanSurface(gPlayers[p][i][j]);
				std::ostringstream f;
				f << DATADIR << "/players/" << (p+1) << '/' << i << '/' << j << ".svg";
				gPlayers[p][i][j] = svgToSurface(f.str());
			}
		}
	}
}

void
Display::cleanSurface(SDL_Surface * &surf)
{
	if ( surf )
	{
		std::map<SDL_Surface *, void *>::iterator i = buffers.find(surf);
		if ( i != buffers.end() )
		{
			free(i->second);
			buffers.erase(i);
		}
		SDL_FreeSurface(surf);
		surf = NULL;
	}
}

void
Display::quit()
{
	if ( ! sDisplay ) return;
	
	cleanSurface(sBackground);
	
	cleanSurface(gMapLayer);
	cleanSurface(gBarrelsLayer);
	cleanSurface(gPlayersLayer);
	
	cleanSurface(gBomb);
	cleanSurface(gExplosion);
	cleanSurface(gBarrel);
	cleanSurface(gWall);
	cleanSurface(gBack);
	
	unsigned int max = Config::getInt("maxPlayers");
	for ( unsigned int p = 0 ; p < max ; ++p )
	{
		for ( unsigned int i = 0 ; i < 4 ; ++i )
		{
			for ( unsigned int j = 0 ; j < ANIM_IMAGES ; ++j )
				cleanSurface(gPlayers[p][i][j]);
		}
	}
	
	//SDL_LockMutex(mUpdate);
	SDL_DestroyMutex(mUpdate);
	
	rsvg_term();
	TTF_Quit();
	sDisplay = NULL;
	if ( SDL_WasInit(SDL_INIT_VIDEO) )
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void
Display::changeFullscreen()
{
	bool beFullscreen = ( Config::getInt("fullscreen") == 1 );
	if ( ( sDisplay ) && ( beFullscreen == isFullscreen ) ) return;
	Uint32 adds = 0;
	if ( beFullscreen )
	{
		adds = SDL_FULLSCREEN;
		width = widthMax;
		height = heightMax;
	}
	else
	{
		width = widthMax * 0.9;
		height = heightMax * 0.9;
	}
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
		cleanSurface(sDisplay);
		sDisplay = tmp;
	}
	if ( height < width )
	{
		gSize = height / gMapSize;
		gZone.x = width - height;
		gZone.y = 0;
	}
	else
	{
		gSize = width / gMapSize;
		gZone.x = 0;
		gZone.y = height - width;
	}
	gZone.w = width - gZone.x;
	gZone.h = height - gZone.y;
	
	
	initSurfaces();
	SDL_UnlockMutex(mUpdate);
}

void
Display::updateDisplay(SDL_Surface *s, Uint16 x, Uint16 y, Uint16 w, Uint16 h)
{
	if ( ! sDisplay ) init();
	SDL_Rect r = { x, y, w, h };
	SDL_LockMutex(mUpdate);
	if ( SDL_MUSTLOCK(sDisplay) )
		SDL_LockSurface(sDisplay);
	SDL_BlitSurface(s, NULL, sDisplay, &r);
	SDL_UpdateRect(sDisplay, x, y, w, h);
	SDL_UnlockSurface(sDisplay);
	SDL_UnlockMutex(mUpdate);
}

void
Display::displayMenu(Menu *menu)
{
	if ( ! sDisplay ) init();
	
	std::vector< std::string> content = menu->getContent();
	unsigned int current = menu->getCurrent();
	int fSize = 2 * content.size();
	
	TTF_Font *fontTitle = TTF_OpenFont(DATADIR"/"FONT_FILE, (height / fSize));
	TTF_Font *fontNormal = TTF_OpenFont(DATADIR"/"FONT_FILE, (height / ( fSize * 2 )));
	
	if ( ( ! fontTitle ) || ( ! fontNormal ) )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
	}
	
	SDL_Surface *textSurface, *sMenu = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_BlitSurface(sBackground, NULL, sMenu, NULL);
	Uint32 dy = ( TTF_FontLineSkip(fontTitle) + ( height / ( fSize * 2 ) ) ), bx = ( width / 2 );
	SDL_Rect r;
	r.x = 0;
	r.y = ( 2 *( height - ( dy * content.size() ) ) / 3 );
	unsigned int e = content.size();
	for ( unsigned int i = 0 ; i < e ; ++i )
	{
		TTF_Font *font = ( i == 0 ) ? ( fontTitle ) : ( fontNormal );
		SDL_Color color = ( i == current ) ? ( highlightColor ) : ( textColor );
		const char *text = content[i].c_str();
		if ( ! ( textSurface = TTF_RenderUTF8_Blended(font, text, color) ) )
			bherr << "Can't display the line" << text << bhendl;
		else
		{
			int w, h;
			TTF_SizeText(font, text, &w, &h);
			r.x = bx - ( w / 2 );
			SDL_BlitSurface(textSurface, NULL, sMenu, &r);
			r.y += dy;
			SDL_FreeSurface(textSurface);
		}
	}
	updateDisplay(sMenu);
	SDL_FreeSurface(sMenu);
	TTF_CloseFont(fontTitle);
	TTF_CloseFont(fontNormal);
}

void
Display::updateScores()
{
	if ( ! sDisplay ) init();
	
	if ( ( gZone.x == 0 ) && ( gZone.y == 0 ) ) return;
	
	SDL_Rect z = {0, 0, 0, 0}, dp = {0, 0, 0, 0}, dt = {0, 0, 0, 0};
	
	Uint32 sSize = 0, nbAll = ( Config::getInt("nbPlayers") + Config::getInt("nbAIs") );
	if ( nbAll < 1 ) return;
	
	TTF_Font *font = TTF_OpenFont(DATADIR"/"FONT_FILE, (gSize*2));
	if ( ! font )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
	}
	
	Uint16 dx = 0, dy = 0;
	int wText, hText;
	TTF_SizeText(font, "00", &wText, &hText);
	if ( width > height )
	{	// Horizontal screen -> Vertical scores
		z.w = gZone.x;
		z.h = height;
		dy = z.h / nbAll;
		sSize = dy / 4;
		dp.x = sSize / 2 - gSize / 2;
		dp.y = 5 * sSize / 4;
		if ( dy < z.w )
		{
			dt.x = dp.y;
			dt.y = sSize / 2 - hText / 2;
		}
		else
			dt.y = dp.y + sSize - hText / 2;
	}
	else
	{	// Vertical screen -> Horizontal scores
		z.w = width;
		z.h = gZone.y;
		dx = z.w / nbAll;
		sSize = dx / 4;
		dp.x = 5 * sSize / 4;
		dp.y = sSize / 2 - gSize / 2;
		if ( dy < z.w )
		{
			dt.x = sSize / 2 - wText / 2;
			dt.y = dp.x;
		}
		else
			dt.x = dp.x + sSize - wText / 2;
	}
	
	cleanSurface(gScoresLayer);
	gScoresLayer = SDL_CreateRGBSurface(flags, z.w, z.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(sBackground, &z, gScoresLayer, NULL);
	
	Sint32 *scores = reinterpret_cast<Sint32 *>(malloc(nbAll * sizeof(Sint32)));
	Sint32 max = -1;
	bool neutral = false;
	std::vector< Player * > players = Player::getPlayers();
	for ( std::vector< Player * >::iterator i = players.begin(), e = players.end() ; i != e ; ++i )
	{
		Sint32 s = (*i)->getScore();
		if ( s > max ) max = s;
		else if ( s == max ) neutral = true;
		scores[(*i)->getId()-1] = s;
	}
	/*
	std::vector< Player * > ais = AI::getAIs();
	for ( std::vector< Player * >::iterator i = ais.begin(), e = ais.end() ; i != e ; ++i )
	{
		Sint32 s = (*i)->getScore();
		if ( s > max ) max = s;
		else if ( s == max ) neutral = true;
		scores[(*i)->getId()-1] = s;
	}
	*/
	
	// Scores heads
	SDL_Surface *sWin = svgToSurface(DATADIR"/scores/win.svg", sSize, sSize);
	SDL_Surface *sLose = svgToSurface(DATADIR"/scores/lose.svg", sSize, sSize);
	SDL_Surface *sEqual = svgToSurface(DATADIR"/scores/equal.svg", sSize, sSize);
		
	for ( unsigned int i = 0 ; i < nbAll ; ++i )
	{
		int s = scores[i];
		
		SDL_Rect
			h = {
				( 2 * sSize / 3 ) + ( i * dx ),
				( 2 * sSize / 3 ) + ( i * dy ),
				sSize,
				sSize
			},
			p = {
				h.x + dp.x,
				h.y + dp.y,
				sSize,
				sSize
			},
			t = {
				h.x + dt.x,
				h.y + dt.y,
				z.w - ( sSize / 2 ),
				z.h - ( sSize / 2 )
			};
		
		SDL_BlitSurface(( s == max ) ? ( ( neutral ) ? ( sEqual ) : ( sWin ) ) : ( sLose ), NULL, gScoresLayer, &h);
		
		SDL_BlitSurface(gPlayers[i][map::DOWN][0], NULL, gScoresLayer, &p);
		
		SDL_Surface *textSurface = NULL;
		std::ostringstream st;
		st << std::setw(2) << std::setfill('0') << s;
		const char *text = st.str().c_str();
		if ( ! ( textSurface = TTF_RenderUTF8_Blended(font, text, textColor) ) )
			bherr << "Can't display the line" << text << bhendl;
		else
		{
			SDL_BlitSurface(textSurface, NULL, gScoresLayer, &t);
			SDL_FreeSurface(textSurface);
		}
	}
	
	
	updateDisplay(gScoresLayer, z);
	
	free(scores);
	TTF_CloseFont(font);
	
	SDL_FreeSurface(sWin);
	SDL_FreeSurface(sLose);
	SDL_FreeSurface(sEqual);
}

void
Display::updateMap()
{
	if ( ( ! gWall ) || ( ! gBack ) ) initSurfaces();
	
	cleanSurface(gMapLayer);
	gMapLayer = SDL_CreateRGBSurface(flags, gZone.w, gZone.h, 32, 0, 0, 0, 0);
	
	map::Coords coords;
	SDL_Rect r;
	r.x = 0;
	r.y = 0;
	r.w = gSize;
	r.h = gSize;
	for(coords.y = 0 ; coords.y < gMapSize ; ++coords.y)
	{
		for(coords.x = 0 ; coords.x < gMapSize ; ++coords.x)
		{
			if ( map::Map::get(coords) == map::INDESTRUCTIBLE )
				SDL_BlitSurface(gWall, NULL, gMapLayer, &r);
			else
				SDL_BlitSurface(gBack, NULL, gMapLayer, &r);
			r.x += gSize;
		}
		r.x = 0;
		r.y += gSize;
	}
	
	updateBarrels();
}

void
Display::updateBarrels()
{
	if ( ! gWall ) initSurfaces();
	SDL_Rect r;
	cleanSurface(gBarrelsLayer);
	gBarrelsLayer = SDL_CreateRGBSurface(flags, gZone.w, gZone.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(gMapLayer, NULL, gBarrelsLayer, NULL);
	map::Coords coords;
	r.y = 0;
	for(coords.y = 0 ; coords.y < gMapSize ; ++coords.y)
	{
		r.x = 0;
		for(coords.x = 0 ; coords.x < gMapSize ; ++coords.x)
		{
			char c = map::Map::get(coords);
			if ( c == map::BARREL )
				SDL_BlitSurface(gBarrel, NULL, gBarrelsLayer, &r);
			else if ( ( c == map::BOMB ) || ( c == map::PLAYONBOMB ) )
				SDL_BlitSurface(gBomb, NULL, gBarrelsLayer, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	updatePlayers();
}

void
Display::updatePlayers()
{
	if ( ! gPlayers[0][0][0] ) initSurfaces();
	
	SDL_Rect r;
	r.w = gSize;
	r.h = gSize;
	cleanSurface(gPlayersLayer);
	gPlayersLayer = SDL_CreateRGBSurface(flags, gZone.w, gZone.h, 32, 0, 0, 0, 0);
	SDL_BlitSurface(gBarrelsLayer, NULL, gPlayersLayer, NULL);
	map::Coords coords;
	std::vector< Player * > players = Player::getPlayers();
	for ( std::vector< Player * >::iterator i = players.begin(), e = players.end() ; i != e ; ++i )
	{
		map::Coords coords = (*i)->getCoords();
		r.x = ( coords.x * gSize );
		r.y = ( coords.y * gSize );
		SDL_BlitSurface(gPlayers[(*i)->getId()-1][(*i)->getOrient()][0], NULL, gPlayersLayer, &r);
	}
	
	updateDisplay(gPlayersLayer, gZone);
}

void
Display::movePlayer(Player *player, map::Direction goTo)
{
	if ( ! sDisplay ) init();
	map::Direction was = player->getOrient();
	if ( player->go(goTo) )
	{
		map::Coords coords = player->getCoords();
		SDL_Rect
			r = {
					coords.x * gSize,
					coords.y * gSize,
					gSize,
					gSize
				},
			l = {
					0,
					0,
					gSize,
					gSize
				};
		
		switch ( goTo )
		{
			case map::DOWN:
				r.y -= gSize;
				l.y = gSize;
			case map::UP:
				r.h += gSize;
			break;
			case map::RIGHT:
				r.x -= gSize;
				l.x = gSize;
			case map::LEFT:
				r.w += gSize;
			break;
		}
		
		SDL_Surface *sPlayer = NULL;
		#if ANIM_IMAGES > 1
		unsigned int anim = 0;
		const Sint16 part = gSize / ANIM_IMAGES;
		const Sint16 cpart = (ANIM_IMAGES-1) * part;
		while ( true )
		{
			SDL_Rect d = {
					0,
					0,
					gSize,
					gSize
				};
			switch ( goTo )
			{
				case map::DOWN:
					d.y = (1+anim)*part;
					if ( static_cast<Uint16>(d.y) >= gSize ) anim = ANIM_IMAGES-1;
				break;
				case map::UP:
					d.y = cpart-anim*part;
					if ( d.y <= 0 ) anim = ANIM_IMAGES-1;
				break;
				case map::RIGHT:
					d.x = (1+anim)*part;
					if ( static_cast<Uint16>(d.x) >= gSize ) anim = ANIM_IMAGES-1;
				break;
				case map::LEFT:
					d.x = cpart-anim*part;
					if ( d.x <= 0 ) anim = ANIM_IMAGES-1;
				break;
			}
			sPlayer = SDL_CreateRGBSurface(flags, r.w, r.h, 32, 0, 0, 0, 0);
			SDL_BlitSurface(gBarrelsLayer, &r, sPlayer, NULL);
			SDL_BlitSurface(gPlayers[player->getId()-1][player->getOrient()][++anim%ANIM_IMAGES], NULL, sPlayer, &d);
			updateDisplay(sPlayer, gZone.x + r.x, gZone.y + r.y, r.w, r.h);
			SDL_FreeSurface(sPlayer);
			if ( anim < ANIM_IMAGES )
				SDL_Delay(ANIM_TIME/ANIM_IMAGES);
			else break;
		}
		#endif // ANIM_IMAGES > 1
		sPlayer = SDL_CreateRGBSurface(flags, r.w, r.h, 32, 0, 0, 0, 0);
		SDL_BlitSurface(gBarrelsLayer, &r, sPlayer, NULL);
		SDL_BlitSurface(gPlayers[player->getId()-1][player->getOrient()][0], NULL, sPlayer, &l);
		updateDisplay(sPlayer, gZone.x + r.x, gZone.y + r.y, r.w, r.h);
		SDL_FreeSurface(sPlayer);
	}
	else if ( player->getOrient() != was )
	{
		map::Coords coords = player->getCoords();
		SDL_Rect r = {
					coords.x * gSize,
					coords.y * gSize,
					gSize,
					gSize
				};
		SDL_Surface *sPlayer = SDL_CreateRGBSurface(flags, r.w, r.h, 32, 0, 0, 0, 0);
		SDL_BlitSurface(gBarrelsLayer, &r, sPlayer, NULL);
		SDL_BlitSurface(gPlayers[player->getId()-1][goTo][0], NULL, sPlayer, NULL);
		updateDisplay(sPlayer, gZone.x + r.x, gZone.y + r.y, r.w, r.h);
		SDL_FreeSurface(sPlayer);
	}
}

void
Display::plantBomb(map::Coords coords)
{
	if ( ! sDisplay ) init();
	SDL_Rect r = {
			gZone.x + coords.x * gSize,
			gZone.y + coords.y * gSize,
			gSize,
			gSize
		};
	
	SDL_BlitSurface(gBomb, NULL, gBarrelsLayer, &r);
	
	updatePlayers();
}

