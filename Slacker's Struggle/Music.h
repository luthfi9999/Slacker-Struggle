#pragma once
#include <SOIL/SOIL.h>

#include <SDL/SDL_mixer.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
class Music 
{
public:
	Music();
	~Music();
	void initBGM();
	void playBGM();
	void stopBGM();
	void resumeBGM();
	bool isAvailable();
	void playSFX(int audio);
	int sfx_channel = -1;
private:
	Mix_Chunk *sfx = NULL, *sfx2 = NULL, *sfx3 = NULL, *sfx4 = NULL, *sfx5 = NULL,*sfx6=NULL;
	Mix_Music *bgm = NULL;

};