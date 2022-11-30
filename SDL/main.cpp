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

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

Box square;
Ball circle;
Box pointer;
SDL_Texture* test = NULL;
Level level;
Camera camera;

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

int randInt(int start, int end) {
	return rand() % end + start;
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

double clamp(int x, int min, int max) {
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}

void resolveWallsCollisions() {
	double scale = camera.getScale();
	double r1 = (square.getPosition().x + square.getWidth());
	double l1 = square.getPosition().x;
	double t1 = square.getPosition().y;
	double b1 = (square.getPosition().y + square.getHeight());

	double cx = circle.getPosition().x + 50;
	double cy = circle.getPosition().y + 50;
	//std::cout << "\nx: " << cx << "y: " << cy;

	bool flag = false;
	for (int i = 0; i < walls.size(); i++) {

		double r2 = (walls.at(i).getPosition().x + walls.at(i).getSize());
		double l2 = walls.at(i).getPosition().x;
		double t2 = walls.at(i).getPosition().y;
		double b2 = (walls.at(i).getPosition().y + walls.at(i).getSize());

		//square collision
		if (r1 > l2 && r2 > l1 && b1 > t2 && b2 > t1) {
			square.resolveBoxCollision(r1, r2, l1, l2, b1, b2, t1, t2);
		}

		double fx = clamp(cx, l2, r2);
		double fy = clamp(cy, t2, b2);


		float distance = sqrt(pow(fx - cx, 2) + pow(fy - cy, 2));
		//std::cout << "\n" << distance;
		if (distance < 50 * scale) {
			circle.resolveBallCollision(cx, cy, l2, r2, t2, b2, 50, fx, fy);
		}

	}
	
}

int main(int argc, char* args[]) {

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
	square.setPosition(100, 200);
	circle.setPosition(200, 200);
	level.loadLevelFromFile("resources/level_design/level_design.txt");
	loadWalls();
	SDL_Event e;
	bool mousePressed = false;
	int mouseX = 0;
	int mouseY = 0;
	bool keyPressed = false;
	

	//printf("\n\nseparate: %d, bounce: %d", SEPARATE, BOUNCE);

	while (true) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return 0;
			}
			getInput(&e, &mousePressed);
		}

		SDL_RenderClear(gRenderer);

		resolveWallsCollisions();
	

		square.smoothenMovement();
		square.move();

		circle.smoothenMovement();
		circle.move();

		camera.positionInMiddle(&square, &circle);
		

		camera.smoothenMovement();
		camera.zoom(&square, &circle);

		camera.move();
		camera.keepInBounds();


		float tempScale = camera.getScale();
		Vector tempCam = camera.getCoords();
		level.renderLevel(tempCam.x, tempCam.y, tempScale);
		square.render(tempCam.x, tempCam.y, tempScale);
		circle.render(tempCam.x, tempCam.y, tempScale);
		pointer.setPosition(camera.getCoords().x, camera.getCoords().y);
		pointer.render(tempCam.x, tempCam.y, tempScale);

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
	if (!square.loadFromFile("resources/square.png")) {
		printf("Failed to load texture1.png!\n");
		return false;
	}
	if (!pointer.loadFromFile("resources/pointer.png")) {
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