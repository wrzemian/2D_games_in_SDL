#pragma once
#include <SDL_image.h>
#include <string>

extern SDL_Renderer* gRenderer;

const int BALLS_COUNT = 15;

const int JOYSTICK_DEAD_ZONE = 8000;

const int SCREEN_HEIGHT = 750;
const int SCREEN_WIDTH = 1200;

const int LEVEL_HEIGHT = 1600;
const int LEVEL_WIDTH = 1600;

struct Vector {
	float x, y;
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