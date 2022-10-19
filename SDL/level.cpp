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
	/*if (air == NULL) {
		SDL_DestroyTexture(air);
		air = NULL;
	}
	if (brick == NULL) {
		SDL_DestroyTexture(brick);
		brick = NULL;
	}
	if (stone == NULL) {
		SDL_DestroyTexture(stone);
		stone = NULL;
	}
	if (air == NULL) {
		SDL_DestroyTexture(water);
		water = NULL;
	}*/
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
	if (!air.loadFromFile("pictures/air.png")) {
		printf("Failed to load air.png!\n");
		return false;
	}

	if (!brick.loadFromFile("pictures/brick.png")) {
		printf("Failed to load brick.png!\n");
		return false;
	}
	if (!stone.loadFromFile("pictures/stone.png")) {
		printf("Failed to load stone.png!\n");
		return false;
	}
	if (!water.loadFromFile("pictures/water.png")) {
		printf("Failed to load water.png!\n");
		return false;
	}

	return true;
}

void Level::renderLevel() {
	int i = 0;
	for (int y = 0; y < HEIGHT; y += 100) {
		for (int x = 0; x < WIDTH; x += 100) {
			if (this->layout.at(i) == '\n') i++;
			switch (this->layout.at(i)) {
			case 'a':
				air.setPosition(x, y);
				air.render();
				break;
			case 'b':
				brick.setPosition(x, y);
				brick.render();
				break;
			case 's':
				stone.setPosition(x, y);
				stone.render();
				break;
			case 'w':
				water.setPosition(x, y);
				water.render();
				break;
			
			}
			i++;
			
			
		}
	}
}