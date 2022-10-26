#pragma once
#include "globals.h"
#include "texture.h"
class Camera {
public:
	Camera();

	~Camera();

	void positionInMiddle(Texture* ball);
	void keepInBounds();
	vector getCoords();
private:
	SDL_Rect camera;
};