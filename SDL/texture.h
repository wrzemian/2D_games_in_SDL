#pragma once
#include <SDL.h>
#include <string>
#include "globals.h"


class Texture {
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render();
	void move();
	void smoothenMovement();
	void setAlpha(Uint8 alpha);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	SDL_Texture* getTexture();
	void setPosition(float X, float Y);
	vector getSpeed();
	void setTargetX(float x);
	void setTargetY(float y);

private:
	SDL_Texture* pTexture;

	int pWidth;
	int pHeight;
	vector speed;
	vector position;
	vector target;
};
