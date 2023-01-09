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

	void renderLevel(int camX, int camY, float scale, int level_size_h, int level_size_w);

	std::string getLayout();

	void changePosition(Vector pos, float scale);

	Vector getPosition();

	private:
		Texture air;
		Texture brick;
		Texture stone;
		Texture water;

		Texture textures[TEXTURES_SIZE];
		std::string layout;

		std::string texturesPaths[TEXTURES_SIZE] = {
	"resources/level_design/tiles/air.png",
	"resources/level_design/tiles/tree_crown.png",
	"resources/level_design/tiles/tree_stump.png",
	"resources/level_design/tiles/cloud.png",
	"resources/level_design/tiles/platform.png",
	"resources/level_design/tiles/mountain_top.png",
	"resources/level_design/tiles/mountain_left.png",
	"resources/level_design/tiles/mountain_right.png",
	"resources/level_design/tiles/mountain_middle.png"
		};

		Vector position = { 0, 0 };
};