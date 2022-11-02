//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "texture.h"
#include "globals.h"
#include <iostream>
#include "level.h";
#include "camera.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

Texture bee;
Texture guy;
SDL_Texture* test = NULL;
Level level;
Camera camera;

const int MAXSPEED = 10;

bool initSDL();
void close();
bool loadTextures();
SDL_Texture* loadTexture(std::string path);
void getInput(SDL_Event *e, bool * mousePressed);

int main(int argc, char* args[]) {

	if (!initSDL()) {
		printf("Failed to load SDL! \n");
		return -1;
	}

	if (!loadTextures()) {
		printf("Failed to load textures! \n");
		return -1;
	}
	printf("loaded succesfully!");

	//ball.setAlpha(255 / 2);
	bee.setPosition(800, 650);
	guy.setPosition(900, 900);
	level.loadLevelFromFile("resources/level_design/level_design.txt");
	SDL_Event e;
	bool mousePressed = false;
	int mouseX = 0;
	int mouseY = 0;
	bool keyPressed = false;
	
	while (true) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return 0;
			}
			getInput(&e, &mousePressed);
		}
		/*if (mousePressed) {
			SDL_GetMouseState(&mouseX, &mouseY);
			guy.setPosition(mouseX - guy.getWidth() / 2, mouseY - guy.getHeight() / 2);
		}*/

		SDL_RenderClear(gRenderer);
		bee.smoothenMovement();
		bee.move();

		guy.smoothenMovement();
		guy.move();

		camera.positionInMiddle(&bee, &guy);
		camera.smoothenMovement();
		camera.move();
		camera.zoom(&bee, &guy);
		camera.keepInBounds();


		float tempScale = camera.getScale();
		vector tempCam = camera.getCoords();
		level.renderLevel(tempCam.x, tempCam.y, tempScale);
		bee.render(tempCam.x, tempCam.y, tempScale);
		guy.render(tempCam.x, tempCam.y, tempScale);


		SDL_RenderPresent(gRenderer);

	}

	close();
	return 0;
}

bool initSDL() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n");
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void close() {
	bee.free();
	guy.free();
	
	//Close game controller
	SDL_JoystickClose(gGameController);
	gGameController = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	printf("IMG quit \n");
	SDL_Quit();
	printf("SDL quit \n");
}


bool loadTextures() {
	if (!bee.loadFromFile("resources/cheems.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}

	if (!guy.loadFromFile("resources/najman.png")) {
		printf("Failed to load texture2.png!\n");
		return false;
	}
	if (!level.loadTextures()) {
		printf("Failed to load level textures!\n");
		return false;
	}

	return true;
}

SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* tempTexture = NULL;
	SDL_Surface* tempSurface = IMG_Load(path.c_str());

	if (tempSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	tempTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (tempTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}
	return tempTexture;
}

void getInput(SDL_Event* e, bool* mousePressed) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		*mousePressed = true;
	}
	if (e->type == SDL_MOUSEBUTTONUP) {
		*mousePressed = false;
	}

	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_UP) {
			bee.setTargetY(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			bee.setTargetY(MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			bee.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			bee.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_UP) {
			bee.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			bee.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			bee.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			bee.setTargetX(0);
		}
	}

	if (e->type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e->jaxis.which == 0)
		{
			//X axis motion
			if (e->jaxis.axis == 0)
			{
				//Left of dead zone
				if (e->jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					guy.setTargetX(-MAXSPEED);
				}
				//Right of dead zone
				else if (e->jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					guy.setTargetX(MAXSPEED);
				}
				else
				{
					guy.setTargetX(0);
				}
			}
			//Y axis motion
			else if (e->jaxis.axis == 1)
			{
				//Below of dead zone
				if (e->jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					guy.setTargetY(-MAXSPEED);
				}
				//Above of dead zone
				else if (e->jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					guy.setTargetY(MAXSPEED);
				}
				else
				{
					guy.setTargetY(0);
				}
			}

		}
	}
}