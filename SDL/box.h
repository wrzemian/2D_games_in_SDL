#pragma once
#include "ball.h"

class Box : public Texture {

public:


	double getSize();

	std::string str();

private:
	double mass = 1;
	double size = 100;
};
