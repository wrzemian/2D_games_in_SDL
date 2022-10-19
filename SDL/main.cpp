//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "texture.h"
#include "globals.h"
#include <iostream>
#include "level.h";


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Texture ball;
Texture square;
SDL_Texture* test = NULL;
Level level;

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

	ball.setAlpha(255 / 2);
	ball.setPosition(100, 100);
	square.setPosition(300, 400);
	level.loadLevelFromFile("pictures/level_design.txt");
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
		if (mousePressed) {
			SDL_GetMouseState(&mouseX, &mouseY);
			square.setPosition(mouseX - square.getWidth() / 2, mouseY - square.getHeight() / 2);
		}

		SDL_RenderClear(gRenderer);
		ball.smoothenMovement();
		ball.move();
		level.renderLevel();
		square.render();
		ball.render();

		//std::cout << "X: " << ball.getSpeed().x << "\n";
		SDL_RenderPresent(gRenderer);

	}

	close();
	return 0;
}

bool initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	gWindow = SDL_CreateWindow("zadanie 1", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(gRenderer, 0xCF, 0xB8, 0x52, 0xFF);

	return true;
}

void close() {
	ball.free();
	square.free();

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
	if (!ball.loadFromFile("pictures/cheems.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}

	if (!square.loadFromFile("pictures/najman.bmp")) {
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
			ball.setTargetY(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			ball.setTargetY(MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			ball.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			ball.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_UP) {
			ball.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			ball.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			ball.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			ball.setTargetX(0);
		}
	}
}