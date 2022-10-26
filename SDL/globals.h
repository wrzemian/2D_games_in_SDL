#pragma once
#include <SDL_image.h>
#include <string>

extern SDL_Renderer* gRenderer;

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 600; 

const int LEVEL_HEIGHT = 1600;
const int LEVEL_WIDTH = 1600;

struct vector {
	float x, y;
};

enum Textures {
	TEXTURES_WATER,
	TEXTURES_AIR,	
	TEXTURES_STONE,
	TEXTURES_BRICK,
	TEXTURES_SIZE
};

;