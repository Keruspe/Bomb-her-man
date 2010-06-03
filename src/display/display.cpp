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

#include "display.hpp"

#include <sstream>
#include <librsvg/rsvg-cairo.h>
#include <cairo.h>

#include "game/player.hpp"

#define FONT_FILE "biolinum.ttf"

using namespace bombherman;

SDL_Surface *Display::sDisplay = NULL;
Uint32 Display::flags = SDL_HWSURFACE;
bool Display::isFullscreen = false;
SDL_mutex *Display::mUpdate = SDL_CreateMutex();

SDL_Color Display::textColor = {255, 255, 255, 0};
SDL_Color Display::highlightColor = {255, 0, 0, 0};
SDL_Color Display::scoreColor = {0, 0, 0, 0};

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
SDL_Surface *Display::gBonuses[NB_BONUSES];
SDL_Surface *Display::gBomb = NULL;
SDL_Surface *Display::gExplosion = NULL;
SDL_Surface *Display::gBarrel = NULL;
SDL_Surface *Display::gTomb[2] = {NULL, NULL};
SDL_Surface *Display::gFloor = NULL;


Uint32 Display::gMapSize = 0;
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
	SDL_Surface *icon = svgToSurface(DATADIR"/graphics/bomb-her-man.svg");
	SDL_WM_SetIcon(icon, NULL);
	SDL_FreeSurface(icon);
	icon = NULL;
	
	
	// sBackground
	cleanSurface(sBackground);
	sBackground = SDL_CreateRGBSurface(flags, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(sBackground, NULL, 0x00444444);
	
	
	
	// gBomb
	cleanSurface(gBomb);
	gBomb = svgToSurface(DATADIR"/graphics/bomb.svg");
	
	
	// gTombs
	cleanSurface(gTomb[0]);
	cleanSurface(gTomb[1]);
	gTomb[0] = svgToSurface(DATADIR"/graphics/tomb1.svg");
	gTomb[1] = svgToSurface(DATADIR"/graphics/tomb2.svg");
	
	// gFloor
	cleanSurface(gFloor);
	gFloor = svgToSurface(DATADIR"/graphics/floor.svg", gZone.w, gZone.h);
	
	// gBarrel
	cleanSurface(gBarrel);
	gBarrel = svgToSurface(DATADIR"/graphics/barrel.svg");
	
	
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
				f << DATADIR << "/graphics/players/" << (p+1) << '/' << i << '/' << j << ".svg";
				gPlayers[p][i][j] = svgToSurface(f.str());
			}
		}
	}
	
	
	/*
	 * Bonuses
	 */
	for ( unsigned int b = 0 ; b < NB_BONUSES ; ++b )
	{
		cleanSurface(gBonuses[b]);
		std::ostringstream f;
		f << DATADIR << "/graphics/bonuses/" << (b+1) << ".svg";
		gBonuses[b] = svgToSurface(f.str());
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
Display::quitGame()
{
	cleanSurface(gMapLayer);
	cleanSurface(gBarrelsLayer);
	cleanSurface(gPlayersLayer);
}

void
Display::quit()
{
	if ( ! sDisplay ) return;
	
	cleanSurface(sBackground);
	
	quitGame();
	
	cleanSurface(gBomb);
	cleanSurface(gExplosion);
	cleanSurface(gBarrel);
	cleanSurface(gTomb[0]);
	cleanSurface(gTomb[1]);
	cleanSurface(gFloor);
	
	unsigned int max = Config::getInt("maxPlayers");
	for ( unsigned int p = 0 ; p < max ; ++p )
	{
		for ( unsigned int i = 0 ; i < 4 ; ++i )
		{
			for ( unsigned int j = 0 ; j < ANIM_IMAGES ; ++j )
				cleanSurface(gPlayers[p][i][j]);
		}
	}
	for ( unsigned int b = 0 ; b < NB_BONUSES ; ++b )
		cleanSurface(gBonuses[b]);
	
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
Display::displayScores()
{
	updateScores(true);
}

void
Display::updateScores(bool final)
{
	if ( ! sDisplay ) init();
	
	Uint32 sSize = 0, nbAll = ( Config::getInt("nbPlayers") + Config::getInt("nbAIs") );
	if ( nbAll < 1 ) return;
	
	
	SDL_Rect
		z = {0, 0, 0, 0},
		dh = {0, 0, 0, 0},
		dp = {0, 0, 2, 2},
		db = {0, 0, 0, 0};
	SDL_Surface *sScoreBack = NULL;
	bool vertical(true);
	if ( final )
	{
		dh.w = z.w = width;
		dh.h = z.h = height;
		if ( height < width )
			vertical = false;
	}
	else
	{
		if ( ( gZone.x == 0 ) && ( gZone.y == 0 ) ) return;
		dh.w = z.w = gZone.x;
		dh.h = z.h = gZone.y;
		if ( z.w == 0 )
			vertical = true;
	}
	
	if ( vertical )
	{	// Vertical scores
		z.h = height;
		dh.y = dh.h = z.h / nbAll;
		sSize = z.w;
		sScoreBack = svgToSurface(DATADIR"/graphics/scores/background-vertical.svg", sSize, sSize);
		db.y = sSize;
	}
	else
	{	// Horizontal scores
		z.w = width;
		dh.x = dh.w = z.w / nbAll;
		sSize = z.h;
		sScoreBack = svgToSurface(DATADIR"/graphics/scores/background-horizontal.svg", sSize, sSize);
		db.x = sSize;
	}
	
	sSize /= 1.7;
	if ( dh.w > dh.h )
	{
		dp.x = sSize;
		dp.y = ( sSize - gSize ) / 2;
		dp.w = 4;
	}
	else
	{
		dp.x = ( sSize - gSize ) / 2;
		dp.y = sSize;
		dp.h = 4;
	}
	
	
	cleanSurface(gScoresLayer);
	gScoresLayer = SDL_CreateRGBSurface(flags, z.w, z.h, 32, 0, 0, 0, 0);
	
	
	SDL_Rect b = {0, 0, 0, 0};
	for ( Uint32 i = 0 ; ( b.x < z.w ) && ( b.y < z.h ) ; ++i )
	{
		SDL_BlitSurface(sScoreBack, NULL, gScoresLayer, &b);
		b.x = (i*db.x);
		b.y = (i*db.y);
	}
	
	SDL_FreeSurface(sScoreBack);
	
	Sint32 *scores = reinterpret_cast<Sint32 *>(malloc(nbAll * sizeof(Sint32)));
	Sint32 max = -10;
	bool neutral(false);
	for ( std::vector< Player * >::iterator i = Player::players.begin(), e = Player::players.end() ; i != e ; ++i )
	{
		Sint32 s = (*i)->getScore();
		if ( s > max ) max = s;
		else if ( s == max ) neutral = true;
		scores[(*i)->getId()-1] = s;
	}
	
	// Scores heads
	SDL_Surface *sWin[] = {
			svgToSurface(DATADIR"/graphics/scores/1/win.svg", sSize, sSize),
			svgToSurface(DATADIR"/graphics/scores/2/win.svg", sSize, sSize)
		};
	SDL_Surface *sLose[] = {
			svgToSurface(DATADIR"/graphics/scores/1/lose.svg", sSize, sSize),
			svgToSurface(DATADIR"/graphics/scores/2/lose.svg", sSize, sSize)
		};
	SDL_Surface *sEqual[] = {
			svgToSurface(DATADIR"/graphics/scores/1/equal.svg", sSize, sSize),
			svgToSurface(DATADIR"/graphics/scores/2/equal.svg", sSize, sSize)
		};
	
	TTF_Font *font = TTF_OpenFont(DATADIR"/"FONT_FILE, (sSize / 6));
	if ( ! font )
	{
		bherr << TTF_GetError() << bhendl;
		throw exceptions::display::NoSDLException("Impossible d'ouvrir la police");
	}
	
	
	for ( unsigned int i = 0 ; i < nbAll ; ++i )
	{
		int s = scores[i];
		
		SDL_Rect h = {
				( ( dh.w - sSize ) / dp.w ) + ( i * dh.x ),
				( ( dh.h - sSize ) / dp.h ) + ( i * dh.y ),
				sSize,
				sSize
			};
		
		SDL_BlitSurface(( s == max ) ? ( ( neutral ) ? ( sEqual[i%2] ) : ( sWin[i%2] ) ) : ( sLose[i%2] ), NULL, gScoresLayer, &h);
		
		
		SDL_Surface *textSurface = NULL;
		std::ostringstream ost;
		ost << s;
		std::string st(ost.str());
		const char *text = st.c_str();
		if ( ! ( textSurface = TTF_RenderUTF8_Blended(font, text, scoreColor) ) )
			bherr << "Can't display the line" << st << bhendl;
		else
		{
			int wText, hText;
			TTF_SizeText(font, text, &wText, &hText);
			SDL_Rect t = {
					h.x + ( sSize - wText ) / 2,
					h.y + sSize * 0.68,
					wText,
					hText
				};
			SDL_BlitSurface(textSurface, NULL, gScoresLayer, &t);
			SDL_FreeSurface(textSurface);
		}
		if ( ! final )
		{
			SDL_Rect p = {
					h.x + dp.x,
					h.y + dp.y,
					gSize,
					gSize
				};
			SDL_BlitSurface(gPlayers[i][map::DOWN][0], NULL, gScoresLayer, &p);
		}
	}
	
	updateDisplay(gScoresLayer, z);
	
	free(scores);
	TTF_CloseFont(font);
	
	SDL_FreeSurface(sWin[0]);
	SDL_FreeSurface(sWin[1]);
	SDL_FreeSurface(sLose[0]);
	SDL_FreeSurface(sLose[1]);
	SDL_FreeSurface(sEqual[0]);
	SDL_FreeSurface(sEqual[1]);
}

void
Display::updateMap()
{
	updateScores();
	
	if ( ( ! gTomb[0] ) || ( ! gTomb[1] ) || ( ! gFloor ) ) initSurfaces();
	
	cleanSurface(gMapLayer);
	gMapLayer = SDL_CreateRGBSurface(flags, gZone.w, gZone.h, 32, 0, 0, 0, 0);
	
	SDL_BlitSurface(gFloor, NULL, gMapLayer, NULL);

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
				SDL_BlitSurface(gTomb[(coords.x+coords.y)%2], NULL, gMapLayer, &r);
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
	if ( ! gMapLayer ) return;
	if ( ( ! gTomb[0] ) || ( ! gTomb[1] ) ) initSurfaces();
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
			else if ( ( c >= '1' ) && ( c <= '6' ) )
				SDL_BlitSurface(gBonuses[c-'1'], NULL, gBarrelsLayer, &r);
			r.x += gSize;
		}
		r.y += gSize;
	}
	
	updateDisplay(gBarrelsLayer, gZone);

	updatePlayers();
}

void
Display::updatePlayers()
{
	if ( ! gPlayers[0][0][0] ) initSurfaces();
	
	SDL_Rect r = {
			0,
			0,
			gSize,
			gSize
		};
	map::Coords coords;
	for ( std::vector< Player * >::iterator i = Player::players.begin(), e = Player::players.end() ; i != e ; ++i )
	{
		if ( ! (*i)->isAlive() ) continue;
		
		coords = (*i)->getCoords();
		r.x = ( coords.x * gSize ) + gZone.x;
		r.y = ( coords.y * gSize ) + gZone.y;
		updateDisplay(gPlayers[(*i)->getId()-1][(*i)->getOrient()][0], r);
	}
}

void
Display::movePlayer(Player * player, map::MoveResult moveResult)
{
	if ( ! sDisplay )
		init();
	SDL_Surface *sPlayer = NULL;
	int p = player->getId() - 1;
	map::Direction goTo = player->getOrient();
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
	#if ANIM_IMAGES > 1
	unsigned int anim = 0;
	const Sint16 part = gSize / ANIM_IMAGES;;
	const Sint16 cpart = (ANIM_IMAGES-1) * part;
	#endif // ANIM_IMAGES > 1
	switch ( moveResult )
	{
		case map::BONUSTAKEN:
			SDL_BlitSurface(gMapLayer, &r, gBarrelsLayer, &r);
		case map::MOVED:
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
			
			#if ANIM_IMAGES > 1
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
				SDL_BlitSurface(gPlayers[p][goTo][++anim%ANIM_IMAGES], NULL, sPlayer, &d);
				updateDisplay(sPlayer, gZone.x + r.x, gZone.y + r.y, r.w, r.h);
				SDL_FreeSurface(sPlayer);
				if ( anim < ANIM_IMAGES )
					SDL_Delay(ANIM_TIME/ANIM_IMAGES);
				else break;
			}
			#endif // ANIM_IMAGES > 1
		break;
		default:
		break;
	}
	sPlayer = SDL_CreateRGBSurface(flags, r.w, r.h, 32, 0, 0, 0, 0);
	if (player->isAlive())
		SDL_BlitSurface(gBarrelsLayer, &r, sPlayer, NULL);
	SDL_BlitSurface(gPlayers[p][goTo][0], NULL, sPlayer, &l);
	updateDisplay(sPlayer, gZone.x + r.x, gZone.y + r.y, r.w, r.h);
	SDL_FreeSurface(sPlayer);
}

void
Display::plantBomb(map::Coords coords)
{
	if ( ! sDisplay ) init();
	SDL_Rect r = {
			coords.x * gSize,
			coords.y * gSize,
			gSize,
			gSize
		};
	
	SDL_BlitSurface(gBomb, NULL, gBarrelsLayer, &r);
	r.x += gZone.x;
	r.y += gZone.y;
	updateDisplay(gBomb, r);
	
	updatePlayers();
}

void
Display::explode(map::Coords, std::vector<map::Coords>)
{
	updateBarrels();
}