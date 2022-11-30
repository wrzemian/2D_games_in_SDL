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

private:

	void recalculateBoxDim();
	

	double mass = 1;
	double size = 100;
	BoxDim d;
};
