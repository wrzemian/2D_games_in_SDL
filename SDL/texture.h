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
	void render(int camX, int camY, float scale);
	void render(SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);

	void move();
	void smoothenMovement();
	void setAlpha(Uint8 alpha);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	SDL_Texture* getTexture();
	void setPosition(float X, float Y);
	Vector getSpeed();
	Vector getPosition();
	void setTargetX(float x);
	void setTargetY(float y);
	void setSpeed(float x, float y);
	void changePosition(Vector delta);
	void setSpeedX(float x);
	void setSpeedY(float y);

	void size(double x, double y);
	void setSize(double x, double y);
	double distance(Texture* other);

	void jump(double v);
	void applyGravity(double dt);

	void setAccelerartion(double x, double y);
	void setAirborne(bool airborne);
	bool getAirborne();
	int getJumpCount();
	void setJumpCount(int jumpCount);
	void quickFalling(double g);


private:
	SDL_Texture* pTexture;

	int pWidth;
	int pHeight;
	Vector speed;
	Vector position;
	Vector target;
	Vector pSize;
	Vector acceleration;
	bool airborne = false;
	int jumpCount = 0;
};
