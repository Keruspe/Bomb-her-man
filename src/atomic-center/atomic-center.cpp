/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include <iostream>

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;
using namespace bombherman::map;
using namespace std;

std::vector<std::vector<int> > AtomicCenter::bombList;

void
AtomicCenter::plantBomb (Coords & c, int playerId)
{
	vector<int> bombInfo;
	bombInfo.push_back (c.x);
	bombInfo.push_back (c.y);
	bombInfo.push_back (playerId);
	SDL_Thread *thread;
	if ((thread = SDL_CreateThread(wait, &bombInfo)) == NULL)
	{
		fprintf(stderr, "Unable to create thread to manage a bomb : %s\n", SDL_GetError());
	}
	cout<<"!!! Attention la bombe va exploser dans 5 secondes ..."<< endl;
	bombInfo.push_back(static_cast<int>(SDL_GetThreadID (thread)));
	bombInfo.push_back (0);
	bombList.push_back (bombInfo);
	SDL_WaitThread (thread, NULL);
}

int
AtomicCenter::wait (void * param)
{
	vector<int> * bombInfo = static_cast<vector<int> * >(param);
	SDL_Delay (5000);
	if (bombInfo->at (4) == 0)
		explode (bombInfo);
	return 0;
}

void
AtomicCenter::explode (vector<int> * bombInfo)
{
	Player player = Player::getPlayer (bombInfo->at (2));
	int range = player.getRange ();
	Coords c;
	c.x = bombInfo->at (0);
	c.y = bombInfo->at(1);
	cout << "Explosion avec les caractéristiques suivantes :" << endl <<
	"Numéro du joueur : " << player.getId () << endl <<
	"Portée de la bombe : " << range << endl <<
	"Position de la bombe : [" << c.x << ", " << c.y << "]" << endl;
}