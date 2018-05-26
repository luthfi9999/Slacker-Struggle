#include "Demo.h"


Demo::Demo()
{
}


Demo::~Demo()
{

}

void Demo::Init()
{
	BuildPlayerSprite();
	BuildCrateSprite();
	buildBackground("start2.jpg");
	buildTopi();
	music->initBGM();

}


void Demo::initAudio()
{
	int flags = MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG;
	if (flags != Mix_Init(flags)) {
		Err("Unable to initialize mixer: " + string(Mix_GetError()));
	}

	int audio_rate = 22050; Uint16 audio_format = AUDIO_S16SYS; int audio_channels = 2; int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		Err("Unable to initialize audio: " + string(Mix_GetError()));
	}

}


void Demo::Update(float deltaTime)
{
	if (IsKeyDown("Quit")) {
		SDL_Quit();
		exit(0);
	}
	music->playBGM();
	if (start) {

		if ((frame_idx == 16 || frame_idx == 17 || frame_idx == 18 || frame_idx == 19) && ypos2 == yAtas) {
			kena = false;
		}
		if (score->getScore() >= 3000) {
			finish = true;
			win = true;
		}
		if (!kena && !finish) {
			updatePosCrate(deltaTime);
			UpdatePlayerSpriteAnim(deltaTime);
			ControlPlayerSprite(deltaTime);
			score->checkScore(xpos2);

		}
		else if (kena && !finish) {
			music->stopBGM();
			int i = 0;
			while (i == 0) {
				if (music->isAvailable())
				{
					music->playSFX(1);
					i = 1;
				}
			}

			while (i == 1) {
				if (music->isAvailable())
				{
					music->playSFX(4);
					i = 2;
				}
			}
			while (i == 2) {
				if (music->isAvailable())
				{
					music->playSFX(5);
					i = 3;
				}
			}		finish = true;

		}
		else {
			if (win) {
				buildBackground("win.jpg");
				music->playSFX(6);

				//render menu menang
			}
			else {
				buildBackground("lose.jpg");
				//render menu kalah

			}
			//selesai
		}
	}
	else if (!start) {
		ControlPlayerSprite(deltaTime);
	}
}


void Demo::Render()
{
	//Setting Viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	if (finish) {
		drawBackground();
	}
	else {
		drawBackground();
		DrawPlayerSprite();
		DrawCrateSprite();
		if (score->getScore() == 2900) {
			drawTopi();
		}

	}


}

void Demo::UpdatePlayerSpriteAnim(float deltaTime)
{
	// Update animation
	frame_dur += deltaTime;
	if (frame_dur > FRAME_DUR) {
		frame_dur = 0;
		if (state == 0) {
			if (counter < 8) {
				frame_idx = walk[counter++];
			}
			else {
				counter = 0;
				frame_idx = walk[counter++];
			}
		}
		else if (state == 1) {
			if (counter < 4) {
				frame_idx = jump[counter++];
			}
		}
		else {
			if (counter < 4) {
				frame_idx = slide[counter++];
			}
			else {
				cSlide++;
				if (cSlide == 3) {
					state = 0;
					counter = 0;
					cSlide = 0;
				}
				slides = false;
			}
		}
		//printf("%i\n", state);
		UseShader(this->program);
		glUniform1i(glGetUniformLocation(this->program, "frameIndex"), frame_idx);
	}
}

void Demo::print(int x, int y, char *string)
{
	//printf("masuk cuk");
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);
	//get the length of the string to display
	int len = (int)strlen(string);

	//loop to display character by character
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
};

void Demo::ControlPlayerSprite(float deltaTime)
{
	walk_anim = false;
	oldxpos = xpos;
	oldypos = ypos;
	if (IsKeyDown("Jump")) {
		if (onGround && !slides) {
			counter = 0;
			onGround = false;
			state = 1;
			naik = true;
			music->playSFX(2);
		}
	}
	if (IsKeyDown("start")) {
		start = true;
		buildBackground("testBG.jpg");
	}
	if (IsKeyDown("Slide")) {
		if (onGround && !slides) {
			counter = 0;
			slides = true;
			state = 2;
			audio = 3;
			music->playSFX(3);
		}
	}

	if (!onGround) {
		if (naik) {
			if (ypos < maxH) {
				naik = false;
			}
			else {

				ypos -= jPower / 3 * deltaTime;
			}
		}
		else {
			if (ypos >= yposGround) {
				state = 0;
				ypos = yposGround;
				onGround = true;
			}
			else {
				//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
				ypos += jPower / 3 * deltaTime;
			}
		}
	}

	//printf("%f %f\n", xpos, ypos);
	//printf("%i\n", state);
	// check collision between bart and crate
	if (IsCollided(xpos, ypos, 115, 100, xpos2, ypos2, frame_width2, frame_height2)) {//0,1=130,3
		if (frame_idx == 0) {
			incx = 5;
		}
		else if (frame_idx == 1) {
			incx = -5;
		}
		else if (frame_idx == 2) {
			incx = 5;
		}
		else if (frame_idx == 3) {
			incx = 25;
		}
		else if (frame_idx == 4) {
			incx = -10;
		}
		else if (frame_idx == 5) {
			incx = -15;
		}
		else if (frame_idx == 6) {
			incx = -18;
		}
		else if (frame_idx == 7) {
			incx = -25;
		}
		else if (frame_idx == 8) {
			incx = -50;
		}
		else if (frame_idx == 9) {
			incx = -10;
			incy = -45;
		}
		else if (frame_idx == 10) {
			incx = -10;
			incy = -45;
		}
		else if (frame_idx == 11) {
			incx = -10;
			incy = -23;
		}
		else if (frame_idx == 12) {
			incx = -10;
			incy = -45;
		}
		else if (frame_idx == 13) {
			incx = -10;
			incy = -45;
		}
		kena = true;
		xpos = oldxpos + incx;
		ypos = oldypos + incy;
	}
}
void Demo::updatePosCrate(float deltaTime) {
	frame_dur += deltaTime;
	if (frame_dur > FRAME_DUR) {
		if (xpos2 < 0) {
			int random = rand() % 2;
			if (random == 1) {
				ypos2 = yAtas;
				ytopi = yBawah;
				pos = 1;
			}
			else {
				ypos2 = yBawah;
				ytopi = yAtas - 100;
				pos = 0;
			}
			xpos2 = 700;
		}
		else {
			speed += incSpeed;
			xpos2 -= speed;
			xtopi = xpos2;
			//printf("%f", speed);
		}
	}
}
void Demo::DrawPlayerSprite() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "ourTexture"), 0);
	// set flip
	glUniform1i(glGetUniformLocation(this->program, "flip"), flip);
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xpos, ypos - 100, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width, frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void Demo::BuildPlayerSprite()
{
	this->program = BuildShader("playerSprite.vert", "playerSprite.frag");

	// Pass n to shader
	UseShader(this->program);
	glUniform1f(glGetUniformLocation(this->program, "n"), 1.0f / NUM_FRAMES);

	// Load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

										   // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("sticky.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width = ((float)width) / NUM_FRAMES / 2.5;
	frame_height = (float)height / 2.5;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));
	//printf("%f     %f", frame_width, frame_height);
	// set sprite position, gravity, velocity
	xpos = 0;
	yposGround = GetScreenHeight() - frame_height;
	ypos = yposGround;
	gravity = 0.05f;
	xVelocity = 0.1f;
	// Add input mapping
	// to offer input change flexibility you can save the input mapping configuration in a configuration file (non-hard code) !
	InputMapping("Move Right", SDLK_RIGHT);
	InputMapping("Move Left", SDLK_LEFT);
	InputMapping("Move Right", SDLK_d);
	InputMapping("Move Left", SDLK_a);
	InputMapping("Move Right", SDL_BUTTON_RIGHT);
	InputMapping("Move Left", SDL_BUTTON_LEFT);
	InputMapping("Move Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	InputMapping("Move Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	InputMapping("Quit", SDLK_ESCAPE);
	InputMapping("start", SDLK_SPACE);
	//InputMapping("Jump", SDLK_SPACE);
	InputMapping("Jump", SDLK_UP);
	InputMapping("Slide", SDLK_DOWN);
	InputMapping("Jump", SDL_CONTROLLER_BUTTON_A);
}
void Demo::buildTopi()
{
	this->PROGRAML = BuildShader("topi.vert", "topi.frag");
	UseShader(this->PROGRAML);

	// Load and create a texture 
	glGenTextures(1, &TEXTUREL);
	glBindTexture(GL_TEXTURE_2D, TEXTUREL); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("topi.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width3 = frame_width2;
	frame_height3 = frame_height2;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOL);
	glGenBuffers(1, &VBOL);
	glGenBuffers(1, &EBOL);

	glBindVertexArray(VAOL);

	glBindBuffer(GL_ARRAY_BUFFER, VBOL);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOL);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->PROGRAML, "projection"), 1, GL_FALSE, value_ptr(projection));

	// set sprite position, gravity, velocity
	xpos2 = 700;
	ypos2 = GetScreenHeight() - frame_height2;
	//printf("%f", ypos2);
}

void Demo::drawTopi()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, TEXTUREL);
	// Activate shader
	UseShader(this->PROGRAML);
	glUniform1i(glGetUniformLocation(this->PROGRAML, "ourTexture"), 4);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xtopi, ytopi - 100, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width3, frame_height3, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->PROGRAML, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOL);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}
void Demo::BuildCrateSprite()
{
	this->program2 = BuildShader("crateSprite.vert", "crateSprite.frag");
	UseShader(this->program2);

	UseShader(this->program2);
	glUniform1f(glGetUniformLocation(this->program2, "n"), 1.0f / 2);
	// Load and create a texture 
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width2 = (float)width;
	frame_height2 = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection));

	// set sprite position, gravity, velocity
	xpos2 = 700;
	ypos2 = GetScreenHeight() - frame_height2;
	//printf("%f", ypos2);
}

void Demo::buildBackground(char* gambar) {
	this->program3 = BuildShader("bg.vert", "bg.frag");
	UseShader(this->program3);

	// Load and create a texture 
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// All upcoming GL_TEXTURE_2D operations now have effect on our texture object

	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image(gambar, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
									 /*frame_widthBack = width;
									 frame_heightBack = height;*/
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "projection"), 1, GL_FALSE, value_ptr(projection));
}
void Demo::drawBackground() {
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// Activate shader
	UseShader(this->program3);
	glUniform1i(glGetUniformLocation(this->program3, "ourTexture"), 3);

	mat4 model;
	// Translate sprite along x-axis
	// Scale sprite 
	model = scale(model, vec3(GetScreenWidth(), GetScreenHeight(), 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO3);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

}

void Demo::DrawCrateSprite() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Activate shader
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 1);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xpos2, ypos2 - 100, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width2, frame_height2, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAO2);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

bool Demo::IsCollided(float x1, float y1, float width1, float height1,
	float x2, float y2, float width2, float height2) {
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2);
}


int main(int argc, char** argv) {

	Engine::Game &game = Demo();
	game.Start("Collision Detection using AABB", 800, 600, false, WindowFlag::WINDOWED, 60, 1);

	return 0;
}
