//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "texture.h"
#include "globals.h"

const int HEIGHT = 480;
const int WIDTH = 620;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

enum codes {
	FAIL = -1,
	SUCCESS = 0
};

Texture texture1;
Texture texture2;
SDL_Texture* test = NULL;

bool initSDL();
void clean();
bool loadTextures();
SDL_Texture* loadTexture(std::string path);



int main(int argc, char* args[]) {

	if (!initSDL()) {
		printf("Failed to load SDL! \n");
		return FAIL;
	}

	if (!loadTextures()) {
		printf("Failed to load textures! \n");
		return FAIL;
	}
	printf("loaded succesfully!");

	SDL_Event e;
	int xPos = 0;
	int dir = 1;
	while (true) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return SUCCESS;
			}
		}

		SDL_RenderClear(gRenderer);

		texture1.render(150, 100);
		texture2.render(xPos, 300);

		if (xPos >= WIDTH) {
			dir = -1;
		}
		if (xPos <= 0) {
			dir = 1;
		}
		xPos += 1 * dir;
		SDL_RenderPresent(gRenderer);

	}

	clean();
	return SUCCESS;
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

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(gRenderer, 0x42, 0xF5, 0xD4, 0xFF);

	return true;
}

void clean() {
	texture1.free();
	texture2.free();

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
	if (!texture1.loadFromFile("pictures/cheems.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}

	if (!texture2.loadFromFile("pictures/najman.bmp")) {
		printf("Failed to load texture2.png!\n");
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