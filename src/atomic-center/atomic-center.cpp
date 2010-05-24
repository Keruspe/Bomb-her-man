/* 
 * File:   AtomicCenter.cpp
 * Author: mogzor
 * 
 * Created on May 24, 2010, 9:36 AM
 */

#include "atomic-center.hpp"

using namespace bombherman;
using namespace bombherman::bomb;

std::vector<std::vector<Uint32> > AtomicCenter::bombList;

void
AtomicCenter::plantBomb (map::Coords & c, int playerId)
{
	std::vector<Uint32> bombInfo;
	bombInfo.push_back (c.x);
	bombInfo.push_back (c.y);
	bombInfo.push_back (playerId);
	bombList.push_back (bombInfo);
}
void
AtomicCenter::explode ()
{
	
}
