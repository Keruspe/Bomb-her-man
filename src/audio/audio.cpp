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

#include "audio.hpp"

using namespace bombherman;

/*
FLAC::Decoder::File Audio::menu;
FLAC::Decoder::File Audio::game;
*/

SDL_AudioSpec Audio::specs;

Uint8 *Audio::audio_chunk = NULL;
Uint32 Audio::audio_len = 0;
Uint8 *Audio::audio_pos = NULL;

void
Audio::init()
{
	Uint32 wasInit = SDL_WasInit(SDL_INIT_EVERYTHING);
	bool initSuccess(true);
	if ( ! wasInit )
		initSuccess = SDL_Init(SDL_INIT_AUDIO) == 0;
	else if ( !( wasInit & SDL_INIT_AUDIO ) )
		initSuccess = SDL_InitSubSystem(SDL_INIT_AUDIO) == 0;
	
	if ( ! initSuccess )
		throw exceptions::audio::NoSDLException("Can't init Audio subsystem of SDL");
	
	

	/* Set the audio format */
	specs.freq = 22050;
	specs.format = AUDIO_S16;
	specs.channels = 2;    /* 1 = mono, 2 = stereo */
	specs.samples = 1024;  /* Good low-latency value for callback */
	specs.callback = &Audio::callback;
	specs.userdata = NULL;

	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&specs, NULL) < 0 )
		throw exceptions::audio::NoSDLException("Couldn't open audio");
	
	//menu.init(DATADIR"/music/menu.flac");
	//game.init(DATADIR"/music/game.flac");
	
	SDL_PauseAudio(0);
}

void
Audio::quit()
{
	SDL_CloseAudio();
	
	if ( SDL_WasInit(SDL_INIT_AUDIO) )
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void
Audio::callback(void *, Uint8 *stream, Uint32 len)
{
	/* Only play if we have data left */
	if ( audio_len == 0 )
		return;

	/* Mix as much data as possible */
	len = ( len > audio_len ? audio_len : len );
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_len -= len;
}