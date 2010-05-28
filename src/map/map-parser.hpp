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

#ifndef _MAP_PARSER_HPP_
#define _MAP_PARSER_HPP_

#include <iostream>
#include <string>

#include "map-utils.hpp"
#include "exceptions/map/bad-element-exception.hpp"

namespace bombherman
{
	namespace map
	{
		class MapParser
		{
		public:
			/// Parse a file to make a map
			/**
			  * @param The path to the file to parse (std::string)
			  * @param The grid in which to copy it (map::Grid)
			  */
			static bool parse(std::string, Grid &);
		};
	}
}

#endif // _MAP_PARSER_HPP_

