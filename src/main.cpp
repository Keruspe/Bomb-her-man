/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
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

#include "bombherman.hpp"
#include "exceptions/exception.hpp"
#include "game/game.hpp"
#include "atomic-center/atomic-center.hpp"
#include "map/map-utils.hpp"

using namespace bombherman;

int
main()
{
	try
	{
		Game::main();
	}
	catch ( exceptions::Exception &e )
	{
		bherr << "Exception: " << e.message() << bhendl;
	}
	Player::newPlayer ();
	Player::newPlayer ();
	Player * player1 = Player::getPlayer (1);
	Player * player2 = Player::getPlayer (2);
	map::Coords c;
	c.x = 2;
	c.y= 3;
	bomb::AtomicCenter::plantBomb (player1, c);
//	c.x = 4;
//	c.y= 4;
//	bomb::AtomicCenter::plantBomb (player2, c);
//	c.x = 5;
//	c.y= 5;
//	bomb::AtomicCenter::plantBomb (player2, c);
//	c.x = 6;
//	c.y= 6;
//	bomb::AtomicCenter::plantBomb (player1, c);
//	c.x = 7;
//	c.y= 7;
//	bomb::AtomicCenter::plantBomb (player2, c);
//	c.x = 4;
//	c.y= 5;
//	bomb::AtomicCenter::plantBomb (player1, c);
//	c.x = 4;
//	c.y= 3;
//	bomb::AtomicCenter::plantBomb (player2, c);
	SDL_Delay (7000);
	delete(player1);
	delete(player2);
	return 0;
}
