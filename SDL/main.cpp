//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "texture.h"
#include "globals.h"
#include <iostream>
#include "level.h";
#include "camera.h"
#include "ball.h"
#include "box.h"
#include <vector>
#include <time.h>

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

Box square;
Ball circle;
Box target;
Box screen;
Texture arrow;
Texture congrats;
SDL_Texture* test = NULL;
Level level;
Camera camera;

int squarePoints = 0;
int circlePoints = 0;

Ball balls[BALLS_COUNT];
bool SEPARATE = true;
bool BOUNCE = true;


std::vector<Box> walls;

const int MAXSPEED = 3;

bool initSDL();
void close();
bool loadTextures();
SDL_Texture* loadTexture(std::string path);
void getInput(SDL_Event *e, bool * mousePressed);

double calculateAngle(Texture* point) {
	Vector tempCam = camera.getCoords();
	Vector vec = { point->getPosition().x - (tempCam.x + 250),
		point->getPosition().y - (tempCam.y + 250) };
	Vector ref = { 0 , 1 };

	double dot = vec.x * ref.x + vec.y * ref.y;
	double det = vec.x * ref.y - vec.y * ref.x;

	return atan2(dot, det) * 180 / M_PI + 90;
}

int randInt(int start, int end) {
	return rand() % end + start;
}

void wait(unsigned timeout)
{
	timeout += std::clock();
	while (std::clock() < timeout) continue;
}

void loadWalls() {
	walls.clear();
	std::string temp = level.getLayout();
	int i = 0;
	for (int y = 0; y < LEVEL_HEIGHT; y += 100) {
		for (int x = 0; x < LEVEL_WIDTH; x += 100) {
			if (temp[i] == '\n') i++;
			if (temp[i] == 'o') {
				Box wall;
				wall.setPosition(x, y);
				walls.push_back(wall);
			}
			i++;
		}
	};
}

bool validate(float possibleX, float possibleY) {
	for (int i = 0; i < walls.size(); i++) {
		if (walls.at(i).getPosition().x == possibleX && walls.at(i).getPosition().y == possibleY) {
			return false;
		}
	}
	return true;
}

void spawnNotOnWalls(Texture* other) {
	bool flagX = true;
	bool flagY = true;
	int possibleX;
	int possibleY;
	do {
		possibleX = randInt(1, 14) * 100;
		possibleY = randInt(1, 14) * 100;

	} while (!validate(possibleX, possibleY));

	other->setPosition(possibleX, possibleY);
}



void resolveCollisions(int id) {
	for (int i = id; i < BALLS_COUNT; i++) {

		if (i == id) { // if there are no bugs in here....
			continue;
		}

		double distance = balls[i].distance(&balls[id]);

		if (distance < balls[i].getRadius() + balls[id].getRadius()) {
			//printf("collision between %d and %d \n", i, id);

			if (SEPARATE)
			{
				balls[i].separate(&balls[id]);
			}

			if (BOUNCE)
			{
				balls[i].resolveCollision(&balls[id]);
			}
		}
	}
}



void resolveWallsCollisions() {
	
	for (int i = 0; i < walls.size(); i++) {
		square.resolveBoxCollision(&walls.at(i));
		circle.resolveBoxCollision(&walls.at(i));
	}
	
}

int main(int argc, char* args[]) {
	srand(time(NULL));
	if (!initSDL()) {
		printf("Failed to load SDL! \n");
		return -1;
	}

	if (!loadTextures()) {
		printf("Failed to load textures! \n");
		return -1;
	}
	printf("loaded succesfully!");

	//ball.setAlpha(255 / 2);
	
	
	//square.setPosition(100, 200);
	//circle.setPosition(200, 200);

	
	SDL_Event e;
	bool mousePressed = false;
	int mouseX = 0;
	int mouseY = 0;
	bool keyPressed = false;
	bool loaded = false;
	bool check = false;

	//printf("\n\nseparate: %d, bounce: %d", SEPARATE, BOUNCE);
	screen.setPosition(200, 200);
	while (true) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return 0;
			}
			getInput(&e, &mousePressed);
		}

		SDL_RenderClear(gRenderer);

		arrow.setPosition(250, 250);
	
		congrats.setPosition(0, 0);

		if (circlePoints + squarePoints == 0 && !loaded) {
			camera.setScale(1);
			level.loadLevelFromFile("resources/level_design/level_design.txt");
			loadWalls();
			//square.setPosition(100, 100);
			//circle.setPosition(1400, 1400);
			//target.setPosition(400, 400);
			spawnNotOnWalls(&square);
			spawnNotOnWalls(&circle);
			spawnNotOnWalls(&target);
			loaded = true;
			check = true;
		}

		if (circlePoints + squarePoints == 1 && !loaded) {
			camera.setScale(1);
			wait(2000);
			level.loadLevelFromFile("resources/level_design/level_design2.txt");
			loadWalls();
			//square.setPosition(100, 100);
			//circle.setPosition(1400, 1400);
			spawnNotOnWalls(&square);
			spawnNotOnWalls(&circle);
			spawnNotOnWalls(&target);
			loaded = true;
			check = true;
		}

		if (circlePoints + squarePoints == 2 && !loaded) {
			camera.setScale(1);
			wait(2000);
			level.loadLevelFromFile("resources/level_design/level_design3.txt");
			loadWalls();
			//square.setPosition(100, 100);
			//circle.setPosition(1400, 1400);
			spawnNotOnWalls(&square);
			spawnNotOnWalls(&circle);
			spawnNotOnWalls(&target);
			loaded = true;
			check = true;
		}

		if (circlePoints + squarePoints == 3 && !loaded) {
			std::cout << "\n\nsquare: " << squarePoints << " circle: " << circlePoints;
			wait(4000);
			circlePoints = 0;
			squarePoints = 0;
			loaded = false;
			check = false;
		}

		resolveWallsCollisions();

		square.smoothenMovement();
		square.move();

		circle.smoothenMovement();
		circle.move();

		//camera.positionInMiddle(&square, &circle);
		

	
		//camera.zoom(&square, &circle);
		camera.adjustCamera(&circle, &square, &screen);
		camera.smoothenMovement();
		camera.move();
		//camera.keepInBounds();


		float tempScale = camera.getScale();
		Vector tempCam = camera.getCoords();

		

		level.renderLevel(tempCam.x, tempCam.y, tempScale);
		square.render(tempCam.x, tempCam.y, tempScale);
		circle.render(tempCam.x, tempCam.y, tempScale);
		target.render(tempCam.x, tempCam.y, tempScale);

		screen.setSize(600, 600);
		screen.setPosition(tempCam.x, tempCam.y);
		//printf("\nX: %f, Y: %f", tempCam.x, tempCam.y);
		//printf("\nangle: %f", calculateAngle(&target));
		if (!target.isColliding(&screen, tempScale)) {
			arrow.render(NULL, calculateAngle(&target), NULL, SDL_FLIP_NONE);
		}
		
		if (check) {
			if (square.resolveBoxCollision(&target)) {
				squarePoints++;
				check = false;
				loaded = false;
				congrats.render();
			}

			if (circle.resolveBoxCollision(&target)) {
				circlePoints++;
				check = false;
				loaded = false;
				congrats.render();
			}
			
			
		}

		

		SDL_RenderPresent(gRenderer);

	}

	close();
	return 0;
}

bool initSDL() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Check for joysticks
		if (SDL_NumJoysticks() < 1)
		{
			printf("Warning: No joysticks connected!\n");
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen(0);
			if (gGameController == NULL)
			{
				printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
			}
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0x3C, 0x73, 0x69, 0xF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void close() {
	square.free();
	circle.free();
	
	//Close game controller
	SDL_JoystickClose(gGameController);
	gGameController = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	IMG_Quit();
	printf("IMG quit \n");
	SDL_Quit();
	printf("SDL quit \n");
}


bool loadTextures() {
	if (!congrats.loadFromFile("resources/congrats.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}
	if (!square.loadFromFile("resources/square.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}
	if (!target.loadFromFile("resources/target.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}

	if (!circle.loadFromFile("resources/ball.png")) {
		printf("Failed to load texture2.png!\n");
		return false;
	}
	if (!level.loadTextures()) {
		printf("Failed to load level textures!\n");
		return false;
	}
	if (!arrow.loadFromFile("resources/arrow.png")) {
		printf("Failed to load arrow.png!\n");
		return false;
	}
	for (int i = 0; i < BALLS_COUNT; i++) {
		if (!balls[i].loadFromFile("resources/ball.png")) {
			printf("Failed to load circle.png!\n");
			return false;
		}
	}

	return true;
}

SDL_Texture* loadTexture(std::string path) {
	SDL_Texture* tempTexture = NULL;
	SDL_Surface* tempSurface = IMG_Load(path.c_str());

	if (tempSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	tempTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	if (tempTexture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}
	return tempTexture;
}

void getInput(SDL_Event* e, bool* mousePressed) {
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		*mousePressed = true;
	}
	if (e->type == SDL_MOUSEBUTTONUP) {
		*mousePressed = false;
	}

	
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		if (e->key.keysym.sym == SDLK_1) {
			SEPARATE = !SEPARATE;
			printf("\n\nseparate: %d, bounce: %d", SEPARATE, BOUNCE);
		}
		if (e->key.keysym.sym == SDLK_2) {
			BOUNCE = !BOUNCE;
			printf("\n\nseparate: %d, bounce: %d", SEPARATE, BOUNCE);
		}
	}

	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_UP) {
			square.setTargetY(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			square.setTargetY(MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			square.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			square.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_UP) {
			square.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_DOWN) {
			square.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			square.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			square.setTargetX(0);
		}
	}

	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_w) {
			circle.setTargetY(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_s) {
			circle.setTargetY(MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_a) {
			circle.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_d) {
			circle.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_w) {
			circle.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_s) {
			circle.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_a) {
			circle.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_d) {
			circle.setTargetX(0);
		}
	}

	if (e->type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e->jaxis.which == 0)
		{
			//X axis motion
			if (e->jaxis.axis == 0)
			{
				//Left of dead zone
				if (e->jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					circle.setTargetX(-MAXSPEED);
				}
				//Right of dead zone
				else if (e->jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					circle.setTargetX(MAXSPEED);
				}
				else
				{
					circle.setTargetX(0);
				}
			}
			//Y axis motion
			else if (e->jaxis.axis == 1)
			{
				//Below of dead zone
				if (e->jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					circle.setTargetY(-MAXSPEED);
				}
				//Above of dead zone
				else if (e->jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					circle.setTargetY(MAXSPEED);
				}
				else
				{
					circle.setTargetY(0);
				}
			}

		}
	}
}