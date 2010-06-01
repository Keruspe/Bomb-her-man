/* 
 * File:   AI.hpp
 * Author: mogzor
 *
 * Created on May 29, 2010, 8:44 PM
 */

#ifndef _AI_HPP
#define	_AI_HPP

#include "exceptions/too-many-players-exception.hpp"
#include "player.hpp"
#include "SDL.h"
#include "bombherman.hpp"

using namespace bombherman;

namespace bombherman
{
	/// A player controlled by AI
	class AI : public Player
	{
	public:
            static void newPlayer ();
        private:
            AI ();
            void init ();
            static int doThings (void *);
        };
}

#endif	/* _AI_HPP */

