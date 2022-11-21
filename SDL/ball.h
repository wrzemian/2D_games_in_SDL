#pragma once
#include <SDL.h>
#include <string>
#include "globals.h"
#include "texture.h"

class Ball : public Texture {

public:

	void resolveCollision(Ball* other);

	void separate(Ball* other);

	void bounceIfOnEdge();

	double getRadius();

	void setRadius(double r);

	std::string str();

private:
	double mass = 1;
	double radius;
};

