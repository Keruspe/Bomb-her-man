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

#include "bombherman.hpp"
#include "display.hpp"

using namespace bombherman;
using namespace bombherman::display;

Display::Display()
{
	/*
	 * Select the best backend that is
	 * compile-time available
	 */
	Logger::putLine("Create display");
	
	#ifdef HAVE_OPENGL
	Logger::putLine("Init OpenGL");
	bBackend = new backends::OpenGL();
	#else
	#ifdef HAVE_SDLMM
	Logger::putLine("Init SDL");
	bBackend = new backends::SDL();
	#else
	#ifdef HAVE_NCURSES
	Logger::putLine("Init NCurses");
	bBackend = new backends::NCurses();
	#else
	Logger::putLine("Init ASCII");
	bBackend = new backends::ASCII();
	#endif
	#endif
	#endif
}

bool
Display::displayMenu(elements::MenuType type)
{
	Logger::putLine("Displaying menu");
	return bBackend->displayMenu(type);
}

bool
Display::displayMap()
{
	Logger::putLine("Displaying map");
	return bBackend->displayMap();
}


Display::~Display()
{
	delete bBackend;
}
