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

	void resolveBoxCollision(double r1, double r2, double l1, double l2, double b1, double b2, double t1, double t2);
	void resolveBallCollision(double cx, double cy, double l, double r, double t, double b, double ro, double fx, double fy);

private:
	SDL_Texture* pTexture;

	int pWidth;
	int pHeight;
	Vector speed;
	Vector position;
	Vector target;
	Vector pSize;
};
