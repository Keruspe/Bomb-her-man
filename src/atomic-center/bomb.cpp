/* 
 * File:   Bomb.cpp
 * Author: mogzor
 * 
 * Created on May 25, 2010, 8:15 PM
 */

#include "bomb.hpp"
#include "map/map.hpp"

using namespace bombherman;
using namespace bombherman::bomb;
using namespace bombherman::map;

SDL_mutex * Bomb::mutex = SDL_CreateMutex ();

Bomb::Bomb (Player * player, Coords coords) : player (player),
		coords (coords),
		exploded (false)
{

	SDL_Thread *thread;
	if ((thread = SDL_CreateThread(wait, this)) == NULL)
		bherr <<  "Unable to create thread to manage a bomb : " << SDL_GetError();
	bhout << "!!! Attention la bombe va exploser dans 5 secondes ..." << bhendl;
	this->threadId = SDL_GetThreadID (thread);
}

Player *
Bomb::getPlayer ()
{
	return this->player;
}

Uint32
Bomb::getThreadId ()
{
	return this->threadId;
}

Coords
Bomb::getCoords ()
{
	return this->coords;
}

bool
Bomb::isExploded ()
{
	return this->exploded;
}

Bomb::~Bomb ()
{
	delete this;
}

int
Bomb::wait (void * param)
{
	Bomb * bomb = static_cast<Bomb * >(param);
	SDL_Delay (5000);
	if (! bomb->exploded)
		explode (bomb);
	return 0;
}

void
Bomb::explode (Bomb * bomb)
{
	SDL_LockMutex (mutex);
	Player player = * bomb->player;
	Coords coords = bomb->coords;
	Uint32 threadId = bomb->threadId;
	int range =player.getRange ();
	bhout << "Explosion avec les caractéristiques suivantes :" << bhendl <<
	"Numéro du joueur : " << bomb->player->getId () << bhendl <<
	"Portée de la bombe : " << range << bhendl <<
	"Position de la bombe : [" << coords.x << ", " << coords.y << "]" << bhendl;
	//parcourons la case de la bombe et les cases à gauche de la bombe
	char item = 'a';
	for(int x = coords.x; x >= (coords.x - range) && x >= 0; x --) {
		std::cout << "Parcours de la case : [" << x << ", " << coords.y << "] : ";
//		item = Map::get(x, coords.y);
		if (item == 'x')
		{
			std::cout << "UN MUR §§§" << std::endl;
			break;
		}
		std::cout << "OK !" << std::endl;
	}
//	for(int x = coords.x + 1; x <= (coords.x + range) && x <= coords.max; x ++) {
//		item = Map::get(x, coords.y);
//		if (item == 'x')
//		{
//			bhout << "UN MUR §§§" << bhendl;
//			break;
//		}
//		bhout << "Parcours de la case : [" << x << ", " << coords.y << "]" << bhendl;
//	}
//	for(int y = coords.y - 1; y >= (coords.y - range) && y >= 0; y --) {
//		item = Map::get(coords.x, y);
//		if (item == 'x')
//		{
//			cout << "UN MUR §§§" << endl;
//			break;
//		}
//		cout << "Parcours de la case : [" << coords.x << ", " << y << "]" << endl;
//	}
//	for(int y = coords.y + 1; y <= (coords.y + range) && y <= coords.max; y ++) {
//		item = Map::get(coords.x, y);
//		if (item == 'x')
//		{
//			cout << "UN MUR §§§" << endl;
//			break;
//		}
//		cout << "Parcours de la case : [" << coords.x << ", " << y << "]" << endl;
//	}
	SDL_UnlockMutex (mutex);
}