#pragma once
#include <SDL_image.h>
#include <string>

extern SDL_Renderer* gRenderer;

const int BALLS_COUNT = 15;

const int JOYSTICK_DEAD_ZONE = 8000;

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 600;

const int LEVEL_HEIGHT = 1600;
const int LEVEL_WIDTH = 1600;


//const double JUMP_H = 0.016;
//const double JUMP_T = 2;

//const double JUMP_V = -200 * (JUMP_H / JUMP_T);
//const double JUMP_G = 2 * (JUMP_H / (JUMP_T * JUMP_T));

struct Vector {
	float x, y;
};

struct BoxDim {
	double r, l, b, t;
};

namespace gl {
	Vector vector(Vector position1, Vector position2);
	Vector scale(Vector vec, double scale);
	Vector normalise(Vector vec);
	double length(Vector vec);
}

enum Textures {
	TEXTURES_WATER,
	TEXTURES_AIR,	
	TEXTURES_STONE,
	TEXTURES_BRICK,
	TEXTURES_GRASS,
	TEXTURES_SIZE
};

;