#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <sstream>
#include "Texture.h"
class Level {
public:
	Level();
	~Level();
	void free();

	bool loadLevelFromFile(std::string path);

	bool loadTextures();

	void renderLevel(int camX, int camY, float scale);

	private:
		Texture air;
		Texture brick;
		Texture stone;
		Texture water;

		Texture textures[TEXTURES_SIZE];
		std::string layout;

		std::string texturesPaths[TEXTURES_SIZE] = {
	"resources/level_design/water.png",
	"resources/level_design/air.png",
	"resources/level_design/stone.png",
	"resources/level_design/brick.png",
	"resources/level_design/grass.png"
		};
};