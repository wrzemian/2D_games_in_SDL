#pragma once
#include"texture.h"
//#include "ball.h"
#include "globals.h"

class Box : public Texture {

public:


	bool resolveBoxCollision(Box* other);

	BoxDim getBoxDim();

	double getSize();

	std::string str();

	void recalculateWallDim();

	bool isColliding(Box* other, float tempScale);

private:

	void recalculateBoxDim();
	void recalculateScreenDim(float tempScale);
	

	double mass = 1;
	double size = 100;
	BoxDim d;
};
