#include "texture.h"
#include <SDL_image.h>
#include "globals.h"
#include <iostream>

Texture::Texture() {
	pTexture = NULL;
	pWidth = 0;
	pHeight = 0;
	pSpeedX = 0;
	pSpeedY = 0;
	pXPos = 0;
	pYPos = 0;
	targetX = 0;
	targetY = 0;
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
	SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0xFF, 0xFF));
	pTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	if (pTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	pWidth = tempSurface->w;
	pHeight = tempSurface->h;

	SDL_FreeSurface(tempSurface);
	return true;
}

void Texture::render() {
	SDL_Rect renderQuad = { int(pXPos), int(pYPos), pWidth, pHeight };
	SDL_RenderCopy(gRenderer, pTexture, NULL, &renderQuad);

}

void Texture::move() {
	pXPos += pSpeedX;
	pYPos += pSpeedY;
}

void Texture::smoothenMovement() {
	float smooth = 0.5f;
	pSpeedX = targetX * (1 - smooth) + pSpeedX * smooth;
	pSpeedY = targetY * (1 - smooth) + pSpeedY * smooth;
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(pTexture, alpha);
}

int Texture::getHeight() {
	return pHeight;
}

int Texture::getWidth() {
	return pWidth;
}

float Texture::getSpeedX() {
	return pSpeedX;
}

float Texture::getSpeedY() {
	return pSpeedY;
}

SDL_Texture* Texture::getTexture() {
	return pTexture;
}


void Texture::setTargetX(float x) {
	targetX = x;
}

void Texture::setTargetY(float y) {
	targetY = y;
}

void Texture::setPosition(float X, float Y) {
	pXPos = X;
	pYPos = Y;
}