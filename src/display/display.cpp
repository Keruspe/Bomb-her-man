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
using namespace bombherman::exceptions::display;

Display::Display()
{
	/*
	 * Select the best backend that is
	 * runtime available, filter at compile-time
	 */
	bhout << "Create display" << bhendl;
	bBackend = NULL;
	
	#ifdef HAVE_OPENGL
	try
	{
		bhout << "Init OpenGL" << bhendl;
		bBackend = new backends::OpenGL();
	}
	catch ( BackendException &e )
	{
		bherr << e.message();
		bBackend = NULL;
	}
	#endif
	
	#ifdef HAVE_SDLMM
	try
	{
		if ( bBackend == NULL )
		{
			bhout << "Init SDL" << bhendl;
			bBackend = new backends::SDL();
		}
	}
	catch ( BackendException &e )
	{
		bherr << e.message();
		bBackend = NULL;
	}
	#endif
	
	#ifdef HAVE_NCURSES
	try
	{
		if ( bBackend == NULL )
		{
			bhout << "Init NCurses" << bhendl;
			bBackend = new backends::NCurses();
		}
	}
	catch ( BackendException &e )
	{
		bherr << e.message();
		bBackend = NULL;
	}
	#endif
	
	try
	{
		if ( bBackend == NULL )
		{
			bhout << "Init ASCII" << bhendl;
			bBackend = new backends::ASCII();
		}
	}
	catch ( BackendException &e )
	{
		bherr << e.message();
		bBackend = NULL;
	}
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
