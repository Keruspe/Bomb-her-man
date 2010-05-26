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

#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

#include "bombherman.hpp"
#include "exceptions/audio/nosdl-exception.hpp"

#include <SDL.h>
#include <FLAC++/decoder.h>

namespace bombherman
{
	class Audio
	{
	public:
		/// Initialize
		static void init();
		/// Shutdown audio system
		static void quit();
		
		
	private:
		void callback(void *, Uint8 *, Uint32);
		
		//static FLAC::Decoder::File menu, game;
		static SDL_AudioSpec specs;
		static Uint8 *audio_chunk;
		static Uint32 audio_len;
		static Uint32 audio_left;
		static Uint8 *audio_pos;
	};
}



#endif // _AUDIO_HPP_

