/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Bomb-her-man
 * Copyright (C) Hugo Mougard 2010 <mogzor@gmail.com>
 * Copyright (C) Kevin Decherf 2010 <kdecherf@gmail.com>
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

#ifndef _AI_HPP
#define	_AI_HPP

#include <SDL.h>

#include "exceptions/too-many-players-exception.hpp"
#include "player.hpp"
#include "bombherman.hpp"

namespace bombherman
{
	/// A player controlled by AI
	class AI :
		public Player
	{
	public:
		/// Create a new AI
		static void newAI();
	private:
		AI();
		void init();
		static int run(void *);
		void findPath();
		std::vector< map::Coords > playerMap;
	};
}

#endif	/* _AI_HPP */

