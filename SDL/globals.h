#pragma once
#include <SDL_image.h>
#include <string>

extern SDL_Renderer* gRenderer;

const int HEIGHT = 800;
const int WIDTH = 800; 

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