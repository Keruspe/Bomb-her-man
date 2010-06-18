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
#include "game/game.hpp"

using namespace bombherman;

// Those defines are tricks to try supporting minGW

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

int
#ifdef __MINGW32__
SDL_main(int argc, char **argv)
#else
main()
#endif // __MINGW32__
{
	#if ENABLE_NLS
		if ( ! setlocale (LC_ALL, "") )
			bherr << "Locale error, won't have i18n" << bhendl;
		else
		{
			if ( ! bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR) )
				bherr << "Gettext error, won't have i18n" << bhendl;
			else
			{
				if ( ! textdomain(GETTEXT_PACKAGE) )
					bherr << "Gettext error, won't have i18n" << bhendl;
				else
				{
					if ( ! bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8") )
						bherr << "Gettext error, maybe won't have i18n" << bhendl;
				}
			}
		}
	#endif
	try
	{
		// Launch the Game
		Game::main();
	}
	catch ( exceptions::Exception & e )
	{
		// Catch our home made exceptions
		bherr << "Exception: " << e.message() << bhendl;
	}
	catch ( std::exception & e )
	{
		// We should never go there
		bherr << "Ouch, an unknown exception was thrown, please report this as a bug : "
			<< bhendl << e.what() << bhendl;
	}
	return 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

