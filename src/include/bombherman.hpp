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

#ifndef _BOMBHERMAN_HPP_
#define _BOMBHERMAN_HPP_

#include <iostream>
#include <iomanip>

#include "config/config.hpp"
#include "exceptions/exception.hpp"
#include "config.h"
#include "nls.hpp"

namespace bombherman
{
	#define bhfile "file " << std::setw(30) << __FILE__ << ", line " << std::setw(8) << __LINE__ << " => "
	#define bhout std::cout << "In " << bhfile
	#define bherr std::cerr << "Error in " << bhfile
	#define bhendl std::endl
}

#endif // _BOMBHERMAN_HPP_
