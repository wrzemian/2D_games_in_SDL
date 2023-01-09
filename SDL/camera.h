#pragma once
#include "globals.h"
#include "texture.h"
#include "ball.h"

class Camera {
public:
	Camera();

	~Camera();

	void adjustCamera(Ball* p1, Box* p2, Box* screen);
	void adjustCameraOnePlayer(Box* p, Box* screen, int level_size_h, int level_size_w);
	float getDeltaX();

	void positionInMiddle(Texture* p1, Texture* p2);
	void keepInBounds(int level_size_h, int level_size_w);
	Vector getCoords();
	void zoom(Texture* p1, Texture* p2);
	float getScale();
	void setScale(float scale);

	void move();
	void smoothenMovement();

private:
	SDL_Rect camera;
	float scale;
	Vector speed;
	Vector target;
	Vector lastPosition = { 0,0 };
	Vector deltaPosition = { 0,0 };
};