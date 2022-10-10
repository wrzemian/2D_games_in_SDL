#include "texture.h"
#include <SDL_image.h>
#include "globals.h"
#include <iostream>

Texture::Texture() {
	pTexture = NULL;
	pWidth = 0;
	pHeight = 0;
	pX = 0;
	pY = 0;
	pRenderRect = { 0, 0, 0, 0 };
}

Texture::~Texture() {
	free();
}

void Texture::free() {
	if (this == NULL) {
		printf("null pointer \n");
		return;
	}
	if (pTexture == NULL) {
		return;
	}
	SDL_DestroyTexture(pTexture);
	pTexture = NULL;
	pWidth = 0;
	pHeight = 0;
}

bool Texture::loadFromFile(std::string path) {
	free();

	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	pTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	if (pTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	pWidth = tempSurface->w;
	pHeight = tempSurface->h;

	pRenderRect.w = pWidth;
	pRenderRect.h = pHeight;

	SDL_FreeSurface(tempSurface);
	return true;
}

void Texture::render(int x, int y) {
	pRenderRect.x = x;
	pRenderRect.y = y;
	SDL_RenderCopy(gRenderer, pTexture, NULL, &pRenderRect);
	//SDL_Renderer, SDL_Texture, NULL, SDL_Rect
									//{ x, y, width, height}
//std::cout << pRenderRect.x << " " << pRenderRect.y << " " << pRenderRect.w << " " << pRenderRect.h << "\n";
}

int Texture::getHeight() {
	return pHeight;
}

int Texture::getWidth() {
	return pWidth;
}

SDL_Texture* Texture::getTexture() {
	return pTexture;
}
