#include <SDL.h>
#include <SDL_image.h>
#include "level.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "globals.h"

Level::Level() {
	layout = "";
}

Level::~Level() {
	this->free();
}

void Level::free() {
	if (this == NULL) {
		printf("null pointer \n");
		return;
	}
}

bool Level::loadLevelFromFile(std::string path) {
	std::ifstream fTemp;
	std::stringstream sTemp;
	fTemp.open(path);
	if (fTemp.is_open()) {
		sTemp << fTemp.rdbuf();
		this->layout = sTemp.str();
		//std::cout << sTemp.str();
		fTemp.close();
		sTemp.clear();
		return true;
	}
	else {
		std::cout << "\nfailed to load level";
		return false;
	}
}

bool Level::loadTextures() {
	/*if (!textures[0].loadFromFile("pictures/air.png")) {
		printf("Failed to load air.png!\n");
		return false;
	}

	if (!textures[1].loadFromFile("pictures/brick.png")) {
		printf("Failed to load brick.png!\n");
		return false;
	}
	if (!textures[2].loadFromFile("pictures/stone.png")) {
		printf("Failed to load stone.png!\n");
		return false;
	}
	if (!textures[3].loadFromFile("pictures/water.png")) {
		printf("Failed to load water.png!\n");
		return false;
	}*/
	for (int i = 0; i < TEXTURES_SIZE; i++) {
		if (!textures[i].loadFromFile(texturesPaths[i])) {
			printf("failed to load %s !\n", texturesPaths[i].c_str());
			return false;
		}
	}

	return true;
}

void Level::renderLevel(int camX, int camY, float scale, int level_size_h, int level_size_w) {
	int i = 0;
	for (int y = 0; y < level_size_h * 100; y += 100) {
		for (int x = 0; x < level_size_w * 100; x += 100) {
			if (this->layout.at(i) == '\n') i++;

			/*textures[this->layout.at(i) - '0'].setPosition(x, y);
			textures[this->layout.at(i) - '0'].render();*/

			switch (this->layout.at(i)) {
			case '.':
				textures[TEXTURES_AIR].setPosition(x, y);
				textures[TEXTURES_AIR].render(camX, camY, scale);
				break;
			case 'o':
				textures[TEXTURES_TREE_CROWN].setPosition(x, y);
				textures[TEXTURES_TREE_CROWN].render(camX, camY, scale);
				break;
			case '|':
				textures[TEXTURES_TREE_STUMP].setPosition(x, y);
				textures[TEXTURES_TREE_STUMP].render(camX, camY, scale);
				break;
			case 'w':
				textures[TEXTURES_CLOUD].setPosition(x, y);
				textures[TEXTURES_CLOUD].render(camX, camY, scale);
				break;
			case '-':
				textures[TEXTURES_PLATFORM].setPosition(x, y);
				textures[TEXTURES_PLATFORM].render(camX, camY, scale);
				break;
			case 't':
				textures[TEXTURES_MOUNTAIN_TOP].setPosition(x, y);
				textures[TEXTURES_MOUNTAIN_TOP].render(camX, camY, scale);
				break;
			case 'l':
				textures[TEXTURES_MOUNTAIN_LEFT].setPosition(x, y);
				textures[TEXTURES_MOUNTAIN_LEFT].render(camX, camY, scale);
				break;
			case 'r':
				textures[TEXTURES_MOUNTAIN_RIGHT].setPosition(x, y);
				textures[TEXTURES_MOUNTAIN_RIGHT].render(camX, camY, scale);
				break;
			case 'm':
				textures[TEXTURES_MOUNTAIN_MIDDLE].setPosition(x, y);
				textures[TEXTURES_MOUNTAIN_MIDDLE].render(camX, camY, scale);
				break;

			}

			i++;
			
			
		}
	}
}

std::string Level::getLayout() {
	return layout;
}

void Level::changePosition(Vector pos, float scale) {
	position.x -= pos.x * scale;
	//position.y -= pos.y * scale;
}

Vector Level::getPosition() {
	return position;
}