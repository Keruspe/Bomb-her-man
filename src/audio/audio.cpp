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
Uint32 Audio::audio_left = 0;
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
	SDL_AudioSpec s;
	s.freq = 96000;
	s.format = AUDIO_S16;
	s.channels = 2;    /* 1 = mono, 2 = stereo */
	s.samples = 4096;  /* Good low-latency value for callback */
	s.callback = &Audio::callback;
	s.userdata = NULL;

	/* Open the audio device, forcing the desired format */
	if ( SDL_OpenAudio(&s, &specs) < 0 )
		throw exceptions::audio::NoSDLException("Couldn't open audio");
	
	//menu.init(DATADIR"/music/menu.flac");
	//game.init(DATADIR"/music/game.flac");
	SDL_AudioSpec *s2;
	if ( ( s2 = SDL_LoadWAV(DATADIR"/music/tetris.wav", &specs, &audio_chunk, &audio_len) ) == NULL )
		bherr << "Putain de bug ! " << SDL_GetError() << bhendl;
	bhout << s.freq << " => " << s2->freq << bhendl;
	bhout << s.format << " => " << s2->format << bhendl;
	bhout << s.channels << " => " << s2->channels << bhendl;
	bhout << s.samples << " => " << s2->samples << bhendl;
	bhout << "Audio length :" << audio_len << bhendl;
	audio_left = audio_len;
	audio_pos = audio_chunk;
	
	SDL_PauseAudio(0);
}

void
Audio::quit()
{
	SDL_FreeWAV(audio_chunk);
	SDL_CloseAudio();
	
	if ( SDL_WasInit(SDL_INIT_AUDIO) )
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void
Audio::callback(void *, Uint8 *stream, Uint32 len)
{
	/* Only play if we have data left */
	if ( audio_left == 0 )
	{
		audio_left = audio_len;
		audio_pos = audio_chunk;
		SDL_PauseAudio(0);
	}
	/* Mix as much data as possible */
	len = ( len > audio_left ? audio_left : len );
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	audio_pos += len;
	audio_left -= len;
}
