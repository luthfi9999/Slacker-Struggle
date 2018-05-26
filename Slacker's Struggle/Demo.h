#ifndef DEMO_H
#define DEMO_H


#include <SOIL/SOIL.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <SDL/SDL_mixer.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <cstdlib>
#include <map>
#include "Game.h"
#include "Score.h"
#include "Music.h"
#include <GLUT\glut.h>
#define NUM_FRAMES 20
#define FRAME_DUR 80
#define FONTSIZE 40
#define FONTNAME "cs_regular.ttf"
using namespace glm;

class Demo :
	public Engine::Game
{
public:
	Demo();
	~Demo();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	float frame_width = 0, frame_height = 0, frame_width2 = 0, frame_height2 = 0, frame_width3 = 0, frame_height3 = 0;
private:
	float frame_dur = 0, oldxpos = 0, xpos = 0, ypos = 0, xpos2 = 0, ypos2 = 0, gravity = 0, xVelocity = 0, yVelocity = 0, yposGround = 0, jPower = 3
		, maxH = 250, colX = 50, colY = 50, oldypos = 0, incy = 0, yBawah = 523, yAtas = 430, incSpeed = 0.05, speed = 45, xtopi = 0, ytopi = 0;
	GLuint VBO, VAO, EBO, texture, program, VBO2, VAO2, EBO2, texture2, program2, program3, program4, VBO3, VAO3, EBO3, texture3, texture4, VBO4, VAO4, EBO4;
	GLuint VBOL, VAOL, EBOL, TEXTUREL, PROGRAML;
	int walk[9] = { 0,1,2,3,4,5,6,7,8 }, state = 0, counter = 0, slide[4] = { 16,17,18,19 };
	int jump[4] = { 9,10,11,12 }, incx = 0, audio = 0;
	bool walk_anim = false, onGround = true, naik = false, kena = false, slides = false, finish = false, win = false, start = false;
	unsigned int frame_idx = 0, flip = 0, pos = 0, cSlide = 0;
	void BuildPlayerSprite();
	Score *score = new Score;
	Music *music = new Music;
	struct Character {
		GLuint TextureID; // ID handle of the glyph texture
		ivec2 Size; // Size of glyph
		ivec2 Bearing; // Offset from baseline to left/top of glyph
		GLuint Advance; // Offset to advance to next glyph
	};
	map<GLchar, Character> Characters;
	void playMusic();
	void initText();
	void renderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);
	void BuildCrateSprite();
	void buildTopi();
	void drawTopi();
	void build_bg();
	void print(int x, int y, char *string);
	void initAudio();
	void draw_bg();
	Mix_Chunk *sfx = NULL, *sfx2 = NULL, *sfx3 = NULL;
	Mix_Music *bgm = NULL;
	int sfx_channel = -1;
	void DrawCrateSprite();
	void buildBackground(char *gambar);
	void drawBackground();
	void updatePosCrate(float deltaTime);
	bool IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
	void DrawPlayerSprite();
	void UpdatePlayerSpriteAnim(float deltaTime);
	void ControlPlayerSprite(float deltaTime);
};
#endif

