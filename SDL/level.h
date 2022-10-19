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

	void renderLevel();

	private:
		Texture air;
		Texture brick;
		Texture stone;
		Texture water;
		std::string layout;
};