#include "camera.h"
#include <iostream>
Camera::Camera() {
	camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	scale = 1;
}

Camera::~Camera() {

}



void Camera::adjustCamera(Texture* p1, Texture* p2) {
	Vector leftTop;
	Vector rightBottom;

	if (p1->getPosition().x < p2->getPosition().x) {
		leftTop.x = p1->getPosition().x;
		rightBottom.x = p2->getPosition().x + 2 * p2->getWidth();
	}
	else {
		leftTop.x = p2->getPosition().x;
		rightBottom.x = p1->getPosition().x + 2 * p1->getWidth();
	}
		

	if (p1->getPosition().y < p2->getPosition().y) {
		leftTop.y = p1->getPosition().y;
		rightBottom.y = p2->getPosition().y + 2*p2->getHeight();
	}
		
	else {
		leftTop.y = p2->getPosition().y;
		rightBottom.y = p1->getPosition().y + 2 * p1->getHeight();
	}
	Vector neededSize = { abs(leftTop.x - rightBottom.x), abs(leftTop.y - rightBottom.y) };
	

	if (neededSize.x * scale > camera.w ) {
		scale *= 0.99;
	}
	if (neededSize.y * scale > camera.h )
	{
		scale *= 0.99;
	}

	if ((neededSize.x + p1->getWidth()) * scale < camera.w && camera.w > SCREEN_WIDTH) {
		scale *= 1.01;
	}
	if ((neededSize.y + p1->getHeight()) * scale < camera.h&& camera.h > SCREEN_HEIGHT)
	{
		scale *= 1.01;
	}

	//std::cout << "\n\nLT: " << leftTop.x << ", " << leftTop.y;
	//std::cout << "\nRB: " << rightBottom.x << ", " << rightBottom.y;


	target.x = leftTop.x;
	target.y = leftTop.y;
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

Vector Camera::getCoords() {
	return { float(camera.x), float(camera.y) };
}

void Camera::zoom(Texture* p1, Texture* p2)
{
	float distance = sqrt(pow(p2->getPosition().x - p1->getPosition().x, 2) + pow(p2->getPosition().y - p1->getPosition().y, 2)) * scale;
	//std::cout << distance << "\n";
	//std::cout << (camera.x + camera.w) / scale << "\n";
	if (distance > 300 
		//&& (camera.x + camera.w) / scale < LEVEL_WIDTH 
		&& (camera.y + camera.h) / scale <= LEVEL_HEIGHT
		)
		scale *= 0.99;
	//if (distance < 100 && scale <= 1)
		//scale *= 1.01;
}

void Camera::setScale(float scale) {
	this->scale = scale;
}

float Camera::getScale() {
	return scale;
}

void Camera::move() {
	camera.x += speed.x;
	//If the dot went too far to the left or right
	if ((camera.x < 0) || (camera.x + camera.w > LEVEL_WIDTH))
	{
		//Move back
		camera.x -= speed.x;
	}
	camera.y += speed.y;
	//If the dot went too far to the left or right
	if ((camera.y < 0) || (camera.y + camera.h > LEVEL_HEIGHT))
	{
		//Move back
		camera.y -= speed.y;
	}
	//std::cout << camera.x << "   " << camera.y << "\n";
}

void Camera::positionInMiddle(Texture* p1, Texture* p2) {
	camera.x = (p1->getPosition().x + p1->getWidth()/2 + p2->getPosition().x + p2->getWidth() / 2) /4;
	camera.y = (p1->getPosition().y + p1->getHeight() / 2 + p2->getPosition().y + p2->getHeight()/2) / 4;
}

void Camera::smoothenMovement() {
	speed.x = (target.x - camera.x - (camera.w / 2.)) / 20.;
	speed.y = (target.y - camera.y - (camera.h / 2.)) / 20.;
}