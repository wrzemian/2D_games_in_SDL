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
Level background1;
Level background2;
Level background3;
Camera camera;

int jumpCounter = 0;

double JUMP_S = 1.5;
double JUMP_H = 150;
double JUMP_D = 350;
double JUMP_V, JUMP_G;

void recalculateV0_G() {
	JUMP_V = 2 * JUMP_H * JUMP_S / JUMP_D;
	JUMP_G = 2 * JUMP_H * JUMP_S * JUMP_S / JUMP_D / JUMP_D;
	square.setAccelerartion(0, JUMP_G);
	printf("\n\nHeight = %F\n", JUMP_H);
	printf("Distance = %F\n", JUMP_D);
	printf("G = %F\n", JUMP_G);
	printf("V0 = %F", JUMP_V);

}


int squarePoints = 0;
int circlePoints = 0;
int level_size_h = 10;
int level_size_w = 36;

Ball balls[BALLS_COUNT];
bool SEPARATE = true;
bool BOUNCE = true;

float backgroundSpeed1 = 0.8f;
float backgroundSpeed2 = 0.5f;
float backgroundSpeed3 = 0.3f;

void printBackgroundSpeeds() {
	printf("\n\nbackgroundSpeed3 = %F\n", backgroundSpeed3);
	printf("backgroundSpeed2 = %F\n", backgroundSpeed2);
	printf("backgroundSpeed1 = %F\n", backgroundSpeed1);	
}

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
	for (int y = 0; y < level_size_h *100; y += 100) {
		for (int x = 0; x < level_size_w *100; x += 100) {
			if (temp[i] == '\n') i++;
			if (temp[i] == '-') {
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
		possibleX = randInt(1, level_size_w - 2) * 100;
		possibleY = randInt(1, level_size_h - 2) * 100;

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



bool resolveWallsCollisions() {
	bool flag = false;
	for (int i = 0; i < walls.size(); i++) {
		if (square.resolveBoxCollision(&walls.at(i))) {
			flag = true;
		}
		circle.resolveBoxCollision(&walls.at(i));
	}
	return flag;
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


	camera.setScale(1);
	//level_size = 16;
	background3.loadLevelFromFile("resources/level_design/background3.txt");
	background2.loadLevelFromFile("resources/level_design/background2.txt");
	background1.loadLevelFromFile("resources/level_design/background1.txt");
	level.loadLevelFromFile("resources/level_design/level_design.txt");
	loadWalls();
	square.setPosition(100, 600);
	recalculateV0_G();
	printBackgroundSpeeds();
	//circle.setPosition(800, 1400);
	//target.setPosition(400, 400);
	//spawnNotOnWalls(&square);
	//spawnNotOnWalls(&circle);
	//spawnNotOnWalls(&target);


	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;



	//printf("\n\nseparate: %d, bounce: %d", SEPARATE, BOUNCE);
	screen.setPosition(200, 200);
	while (true) {

		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				return 0;
			}
			getInput(&e, &mousePressed);
		}

		SDL_RenderClear(gRenderer);

		square.move();

	
		
		square.applyGravity(deltaTime);
		square.quickFalling(JUMP_G);
		//std::cout << "\nY: " << square.getSpeed().y;
		//square.smoothenMovement();
		//std::cout << "\n" << jumpCounter;
		if (resolveWallsCollisions() && jumpCounter > 1) {
			//std::cout << "\n\n clearing";
			jumpCounter = 0;
		}

		
		
		//std::cout << "\nspeedX: " << square.getSpeed().x << "speedY: " << square.getSpeed().y;


		

	
		camera.adjustCameraOnePlayer(&square, &screen, level_size_h, level_size_w);
		camera.smoothenMovement();
		camera.move();


		float tempScale = camera.getScale();
		Vector tempCam = camera.getCoords();
		
		background3.changePosition(camera.getDelta(), backgroundSpeed3);
		background2.changePosition(camera.getDelta(), backgroundSpeed2);
		background1.changePosition(camera.getDelta(), backgroundSpeed1);
		
		
		background3.renderLevel(background3.getPosition().x, tempCam.y, tempScale, level_size_h, level_size_w);
		background2.renderLevel(background2.getPosition().x, tempCam.y, tempScale, level_size_h, level_size_w);
		background1.renderLevel(background1.getPosition().x, tempCam.y, tempScale, level_size_h, level_size_w);
		level.renderLevel(tempCam.x, tempCam.y, tempScale,level_size_h, level_size_w);

		square.render(tempCam.x, tempCam.y, tempScale);
		//circle.render(tempCam.x, tempCam.y, tempScale);
		//target.render(tempCam.x, tempCam.y, tempScale);

		screen.setSize(600, 600);
		screen.setPosition(tempCam.x, tempCam.y);
		//printf("\nX: %f, Y: %f", tempCam.x, tempCam.y);
		//printf("\nangle: %f", calculateAngle(&target));
		/*if (!target.isColliding(&screen, tempScale)) {
			arrow.render(NULL, calculateAngle(&target), NULL, SDL_FLIP_NONE);
		}*/
		
		//if (check) {
		//	if (square.resolveBoxCollision(&target)) {
		//		squarePoints++;
		//		check = false;
		//		loaded = false;
		//		congrats.render();
		//	}

		//	if (circle.resolveBoxCollision(&target)) {
		//		circlePoints++;
		//		check = false;
		//		loaded = false;
		//		congrats.render();
		//	}
		//}

		

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
				SDL_SetRenderDrawColor(gRenderer, 0x34, 0xEB, 0xEB, 0xF);

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
	if (!background3.loadTextures()) {
		printf("Failed to load level textures!\n");
		return false;
	}
	if (!background2.loadTextures()) {
		printf("Failed to load level textures!\n");
		return false;
	}
	if (!background1.loadTextures()) {
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


	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_SPACE && e->key.repeat == 0 && square.getJumpCount() < 2) {
			square.jump(JUMP_V);
		}
		/*if (e->key.keysym.sym == SDLK_s) {
			square.setTargetY(MAXSPEED);
		}*/
		if (e->key.keysym.sym == SDLK_a) {
			square.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_d) {
			square.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		if (e->key.keysym.sym == SDLK_w) {
			square.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_s) {
			square.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_a) {
			square.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_d) {
			square.setTargetX(0);
		}
	}



	
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		/*if (e->key.keysym.sym == SDLK_o) {
			JUMP_H += 50;
			recalculateV0_G();
		}


		if (e->key.keysym.sym == SDLK_p) {
			JUMP_H -= 50;
			recalculateV0_G();
		}

		if (e->key.keysym.sym == SDLK_k) {
			JUMP_D += 50;
			recalculateV0_G();
		}

		if (e->key.keysym.sym == SDLK_l) {
			JUMP_D -= 50;
			recalculateV0_G();
		}*/


		if (e->key.keysym.sym == SDLK_o) {
			backgroundSpeed3 -= 0.1;
			printBackgroundSpeeds();
		}
		if (e->key.keysym.sym == SDLK_p) {
			backgroundSpeed3 += 0.1;
			printBackgroundSpeeds();
		}

		if (e->key.keysym.sym == SDLK_k) {
			backgroundSpeed2 -= 0.1;
			printBackgroundSpeeds();
		}
		if (e->key.keysym.sym == SDLK_l) {
			backgroundSpeed2 += 0.1;
			printBackgroundSpeeds();
		}

		if (e->key.keysym.sym == SDLK_n) {
			backgroundSpeed1 -= 0.1;
			printBackgroundSpeeds();
		}
		if (e->key.keysym.sym == SDLK_m) {
			backgroundSpeed1 += 0.1;
			printBackgroundSpeeds();
		}
	}


	if (e->type == SDL_KEYDOWN) {
		if (e->key.keysym.sym == SDLK_SPACE && e->key.repeat == 0) {
			circle.jump(JUMP_V);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			circle.setTargetX(-MAXSPEED);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
			circle.setTargetX(MAXSPEED);
		}
	}
	if (e->type == SDL_KEYUP) {
		/*if (e->key.keysym.sym == SDLK_UP) {
			circle.setTargetY(0);
		}*/
		if (e->key.keysym.sym == SDLK_DOWN) {
			circle.setTargetY(0);
		}
		if (e->key.keysym.sym == SDLK_LEFT) {
			circle.setTargetX(0);
		}
		if (e->key.keysym.sym == SDLK_RIGHT) {
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