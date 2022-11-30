#pragma once
#include "ball.h"

class Box : public Texture {

public:

	void resolveCollisionWithBall(Ball* other);

	void resolveCollisionWithBox(Box* other);

	double getSize();

	std::string str();

private:
	double mass = 1;
	double size = 100;
};
