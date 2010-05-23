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

#include "events.hpp"

using namespace bombherman;
using namespace bombherman::events;
using namespace bombherman::exceptions::events;

Events::Events()
{
	/*
	 * Select the best backend that is
	 * compile-time available, filter at compile-time
	 */
	bhout << "Prepare to listen events" << bhendl;
	bBackend = NULL;
	
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
	
	#ifdef HAVE_READLINE
	try
	{
		if ( bBackend == NULL )
		{
			bhout << "Init ASCII" << bhendl;
			bBackend = new backends::ReadLine();
		}
	}
	catch ( BackendException &e )
	{
		bherr << e.message();
		bBackend = NULL;
	}
	#endif
}

#include <cstdlib>

void
Events::main()
{
	sleep(5);
}
