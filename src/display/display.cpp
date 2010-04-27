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

using namespace bombherman;
using namespace bombherman::display;

Display::Display()
{
	/*
	 * Select the best backend that is
	 * compile-time available
	 */
	bhout << "Create display" << bhendl;
	
	#ifdef HAVE_OPENGL
	bhout << "Init OpenGL" << bhendl;
	bBackend = new backends::OpenGL();
	#else
	#ifdef HAVE_SDLMM
	bhout << "Init SDL" << bhendl;
	bBackend = new backends::SDL();
	#else
	#ifdef HAVE_NCURSES
	bhout << "Init NCurses" << bhendl;
	bBackend = new backends::NCurses();
	#else
	bhout << "Init ASCII" << bhendl;
	bBackend = new backends::ASCII();
	#endif
}

void
Display::displayMenu(elements::Menu::Type type)
{
	bhout << "Displaying menu" << bhendl;
	bBackend->displayMenu(type);
}

void
Display::displayMap()
{
	bhout << "Displaying map" << bhendl;
	bBackend->displayMap();
}


Display::~Display()
{
	delete bBackend;
}
