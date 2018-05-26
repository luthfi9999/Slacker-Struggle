#include "Music.h"
#include <cstdio>
Music::Music()
{

}

Music::~Music()
{
	if (Mix_Playing(sfx_channel) == 0) {
		Mix_FreeChunk(sfx);
		Mix_FreeChunk(sfx2);
		Mix_FreeChunk(sfx3);
	}
	if (bgm != NULL) {
		Mix_FreeMusic(bgm);
	}
	Mix_CloseAudio();
}

bool Music::isAvailable()
{
	return(Mix_Playing(sfx_channel) == 0);
}

void Music::initBGM()
{
	//if (Mix_Playing(sfx_channel) == 0) {
	//	Mix_FreeChunk(sfx);
	//	Mix_FreeChunk(sfx2);
	//	Mix_FreeChunk(sfx3);
	//}
	//if (bgm != NULL) {
	//	Mix_FreeMusic(bgm);
	//}
	Mix_CloseAudio();
	int flags = MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG;
	Mix_Init(flags);
	int audio_rate = 22050; Uint16 audio_format = AUDIO_S16SYS; int audio_channels = 2; int audio_buffers = 4096;
	Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
	sfx = Mix_LoadWAV("crash1.wav");
	sfx2 = Mix_LoadWAV("jump.wav");
	sfx3 = Mix_LoadWAV("slide.wav");
	sfx4 = Mix_LoadWAV("crash2.wav");
	sfx5 = Mix_LoadWAV("crash3.wav");
	sfx6 = Mix_LoadWAV("win.wav");
	bgm = Mix_LoadMUS("bgm.wav");
	if (bgm == NULL) {
		printf("error pak");
	}
}

void Music::playBGM()
{
	if (Mix_PlayingMusic() == 0) {
		printf("played");
		Mix_PlayMusic(bgm, -1);
	}

}

void Music::resumeBGM()
{
	Mix_ResumeMusic();
}

void Music::stopBGM()
{
	Mix_PauseMusic();
}

void Music::playSFX(int audio)
{
		if (audio == 1)
		{
			//printf("%i", sfx_channel);
			sfx_channel = Mix_PlayChannel(-1, sfx, 0);

		}
		else if (audio == 2)
		{
			sfx_channel = Mix_PlayChannel(-1, sfx2, 0);
		}
		else if (audio == 3)
		{
			sfx_channel = Mix_PlayChannel(-1, sfx3, 0);
		}
		else if(audio==4)
		{
			sfx_channel = Mix_PlayChannel(-1, sfx4, 0);
		}
		else if (audio == 5)
		{
			sfx_channel = Mix_PlayChannel(-1, sfx5, 0);
		}
		else if (audio == 6)
		{
			sfx_channel = Mix_PlayChannel(-1, sfx6, 0);
		}
}