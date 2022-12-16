#include "texture.h"
#include <SDL_image.h>
#include "globals.h"
#include <iostream>

Texture::Texture() {
	pTexture = NULL;
	pWidth = 0;
	pHeight = 0;
	speed = { 0, 0 };
	target = { 0, 0 };
	position = { 0, 0 };
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
	SDL_Rect renderQuad = { position.x, position.y, pWidth, pHeight};
	SDL_RenderCopy(gRenderer, pTexture, NULL, &renderQuad);

}

void Texture::render(int camX, int camY, float scale) {
	//scale -= 0.2;
	//camX -= 100;
	SDL_Rect renderQuad = { int(position.x - camX) * scale, int(position.y - camY) * scale, pWidth * scale, pHeight * scale };
	SDL_RenderCopy(gRenderer, pTexture, NULL, &renderQuad);

}

void Texture::render(SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { position.x, position.y, pWidth, pWidth };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, pTexture, clip, &renderQuad, angle, center, flip);
}


void Texture::move() {
	position.x += speed.x;
	//If the dot went too far to the left or right
	if ((position.x < 0) )
	{
		//Move back
		position.x -= speed.x;
	}
	//position.y += speed.y;
	////If the dot went too far to the left or right
	//if ((position.y < 0))
	//{
	//	//Move back
	//	position.y -= speed.y;
	//}
}


void Texture::jump() {
	speed.y = float(JUMP_V/1.0f);
	std::cout << " \nnowe: " << speed.y;
}

void Texture::applyGravity(double dt) {
	
	position.y += speed.y * dt + JUMP_G  * dt * dt /2;
	speed.y += JUMP_G * dt /2;

}

void Texture::smoothenMovement() {
	float smooth = 0.5f;
	speed.x = target.x * (1 - smooth) + speed.x * smooth;
	speed.y = target.y * (1 - smooth) + speed.y * smooth;
	if (abs(speed.x) < 0.0001f) {
		speed.x = 0;
	}
	if (abs(speed.y) < 0.0001f) {
		speed.y = 0;
	}
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

Vector Texture::getSpeed() {
	return speed;
}

SDL_Texture* Texture::getTexture() {
	return pTexture;
}


void Texture::setTargetX(float x) {
	target.x = x;
}

void Texture::setTargetY(float y) {
	target.y = y;
}

void Texture::setPosition(float X, float Y) {
	position.x = X;
	position.y = Y;
}

Vector Texture::getPosition() {
	return position;
}

void Texture::setSpeed(float x, float y) {
	speed.x = x;
	speed.y = y;
}

void Texture::changePosition(Vector delta)
{
	this->position.x += delta.x;
	this->position.y += delta.y;
}

void Texture::setSpeedX(float x)
{
	this->setSpeed(x, this->getSpeed().y);

}

void Texture::setSpeedY(float y)
{
	this->setSpeed(this->getSpeed().x, y);

}

void Texture::size(double x, double y)
{
	this->setSize(x, y);
	//this->getTexture()->setSize(x, y);
}

void Texture::setSize(double x, double y) {
	this->pSize.x = x;
	this->pSize.y = y;
}

double Texture::distance(Texture* other)
{
	double deltaX = this->getPosition().x - other->getPosition().x;
	double deltaY = this->getPosition().y - other->getPosition().y;

	return sqrt(deltaX * deltaX + deltaY * deltaY);
}



