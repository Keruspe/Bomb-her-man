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

#ifndef _NLS_HPP_
#define _NLS_HPP_

#if ENABLE_NLS
	#include <libintl.h>
	#define _(x) gettext(x)
	#ifdef gettext_noop
		#define N_(x) gettext_noop(x)
	#else
		#define N_(x) (x)
	#endif
#else
	#define _(x) (x)
	#define ngettext(s, p, n) ((n == 1) ? (s) : (p))
	#define N_(x) (x)
#endif // ENABLE_NLS

#endif // _NLS_HPP_

