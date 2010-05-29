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

#ifndef _DISPLAY_NOSVG_EXCEPTION_HPP_
#define _DISPLAY_NOSVG_EXCEPTION_HPP_

#include "exceptions/exception.hpp"

namespace bombherman
{
	namespace exceptions
	{
		namespace display
		{
			/// Exception thrown when we cannot handle SVGs
			class NoSVGException :
				public Exception
			{
			public:
				/// Constructor
				/**
				 * @param m The message
				 */
				NoSVGException(const std::string & m) throw() :
					Exception("No SVG:" + m) {}
				
				/// Destructor
				virtual ~NoSVGException() throw() {}
			};
		}
	}
}

#endif // _DISPLAY_NOSVG_EXCEPTION_HPP_

