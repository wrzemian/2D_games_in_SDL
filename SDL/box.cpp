#include "box.h"

void Box::jump() {

}





bool Box::isColliding(Box* screen, float tempScale) {
	this->recalculateBoxDim();
	screen->recalculateScreenDim(tempScale);
	/*printf("\ncamR: %f, camL : %f, camT: %f, camB : %f", 
		screen->d.r, screen->d.l, screen->d.t, screen->d.b);
	printf("\ntarR: %f, tarL : %f, tarT: %f, tarB : %f",
		this->d.r, this->d.l, this->d.t, this->d.b);*/

	double left = this->d.r - screen->d.l;
	double right = screen->d.r - this->d.l;
	double top = this->d.b - screen->d.t;
	double bottom = screen->d.b - this->d.t;

	if (left - 50 > 0 && right  - 50 > 0 && top - 50 > 0 && bottom - 50 > 0) {
		//printf("\nna ekranie");
		return true;
	}
		
	else {
		//printf("\npoza ekranen");
		return false;
	}
}


bool Box::resolveBoxCollision(Box* other) {
	this->recalculateBoxDim();
	other->recalculateWallDim();

	double left = this->d.r - other->d.l;
	double right = other->d.r - this->d.l;
	double top = this->d.b - other->d.t;
	double bottom = other->d.b - this->d.t;

	bool flag = false;

	if (left > 0 && right > 0 && top > 0 && bottom > 0) {
		//printf("dupa");
		flag = true;
		Vector separate;
		if (left < right) {
			separate.x = -left;
		}
		else {
			separate.x = right;
		}
		if (top < bottom) {
			separate.y = -top;
		}
		else {
			separate.y = bottom;
		}

		if (abs(separate.x) < abs(separate.y)) {
			separate.y = 0;
		}
		else if (abs(separate.x) > abs(separate.y)) {
			separate.x = 0;
		}
		this->changePosition(separate);
	}

	return flag;

	
}


void Box::recalculateBoxDim() {
	d.l = this->getPosition().x;
	d.r = d.l + this->getWidth();
	
	d.t = this->getPosition().y;
	d.b = d.t + this->getHeight();
}

void Box::recalculateScreenDim(float tempScale) {
	d.l = this->getPosition().x;
	d.r = d.l + 600 / tempScale;

	d.t = this->getPosition().y;
	d.b = d.t + 600 / tempScale;
}

void Box::recalculateWallDim() {
	d.l = this->getPosition().x;
	d.r = d.l + this->getSize();

	d.t = this->getPosition().y;
	d.b = d.t + this->getSize();
}

BoxDim Box::getBoxDim() {
	return d;
}

double Box::getSize() {
	return size;
}

std::string Box::str() {
	return "dupa";
}