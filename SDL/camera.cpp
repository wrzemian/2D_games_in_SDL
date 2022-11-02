#include "camera.h"
#include <iostream>
Camera::Camera() {
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	scale = 1;
}

Camera::~Camera() {

}

void Camera::positionInMiddle(Texture* p1, Texture* p2) {
	camera.x = (p1->getPosition().x + p2->getPosition().x) / 2 - SCREEN_HEIGHT/2;
	camera.y = (p1->getPosition().y + p2->getPosition().y) / 2 - SCREEN_WIDTH/2;
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
	if (camera.x > LEVEL_WIDTH - camera.w / scale )
	{
		camera.x = LEVEL_WIDTH - camera.w / scale;
	}
	if (camera.y > LEVEL_HEIGHT - camera.h / scale)
	{
		camera.y = LEVEL_HEIGHT - camera.h / scale;
	}
}

vector Camera::getCoords() {
	return { float(camera.x), float(camera.y) };
}

void Camera::zoom(Texture* p1, Texture* p2)
{
	float distance = sqrt(pow(p2->getPosition().x - p1->getPosition().x, 2) + pow(p2->getPosition().y - p1->getPosition().y, 2)) * scale;
	//std::cout << distance << "\n";
	//std::cout << (camera.x + camera.w) / scale << "\n";
	if (distance > 400 && (camera.x + camera.w) / scale < LEVEL_WIDTH && (camera.y + camera.h) / scale < LEVEL_HEIGHT)
		scale *= 0.99;
	if (distance < 100)
		scale *= 1.01;
}

float Camera::getScale() {
	return scale;
}