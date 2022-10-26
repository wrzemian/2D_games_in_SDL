#include "camera.h"

Camera::Camera() {
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

Camera::~Camera() {

}

void Camera::positionInMiddle(Texture* ball) {
	camera.x = (ball->getPosition().x + ball->getWidth() / 2) - SCREEN_WIDTH / 2;
	camera.y = (ball->getPosition().y + ball->getHeight() / 2) - SCREEN_HEIGHT / 2;
}

void Camera::keepInBounds() {
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	if (camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

vector Camera::getCoords() {
	return { float(camera.x), float(camera.y) };
}