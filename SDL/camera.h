#pragma once
#include "globals.h"
#include "texture.h"
class Camera {
public:
	Camera();

	~Camera();

	void positionInMiddle(Texture* p1, Texture* p2);
	void keepInBounds();
	vector getCoords();
	void zoom(Texture* p1, Texture* p2);
	float getScale();

	void move();
	void smoothenMovement();

private:
	SDL_Rect camera;
	float scale;
	vector speed;
	vector target;
};