#pragma once
#include <SDL.h>
#include <string>
#include "globals.h"
#include "texture.h"
#include "box.h"

class Ball : public Texture {

public:

	void resolveCollision(Ball* other);

	void separate(Ball* other);

	void bounceIfOnEdge();

	double getRadius();

	void setRadius(double r);

	std::string str();

	bool resolveBoxCollision(Box* other);

private:

	void recalculateBallMid();

	double mass = 1;
	double radius;
	Vector mid;
};

