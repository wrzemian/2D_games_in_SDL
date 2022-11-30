#pragma once
#include "globals.h"
#include "texture.h"
class Camera {
public:
	Camera();

	~Camera();

	void adjustCamera(Texture* p1, Texture* p2);

	void positionInMiddle(Texture* p1, Texture* p2);
	void keepInBounds();
	Vector getCoords();
	void zoom(Texture* p1, Texture* p2);
	float getScale();

	void move();
	void smoothenMovement();

private:
	SDL_Rect camera;
	float scale;
	Vector speed;
	Vector target;

};