#pragma once
#include <SDL.h>
#include <string>


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

	void smoothenX();
	void smoothenY();
	void setAlpha(Uint8 alpha);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	SDL_Texture* getTexture();
	void setSpeed(int X, int Y);
	void setPosition(int X, int Y);
	float getSpeedX();
	float getSpeedY();
	void setSpeedX(int x);

	void setSpeedY(int y);
	void setTargetX(int x);

	void setTargetY(int y);

private:
	SDL_Texture* pTexture;

	int pWidth;
	int pHeight;
	float pSpeedX;
	float pSpeedY;
	float pXPos;
	float pYPos;
	float targetX;
	float targetY;
};
