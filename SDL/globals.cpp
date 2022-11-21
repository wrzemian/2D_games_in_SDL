#include "globals.h"

Vector gl::vector(Vector position1, Vector position2)
{
	float x = position1.x - position2.x;
	float y = position1.y - position2.y;
	return { x, y };
}

Vector gl::scale(Vector vec, double scale)
{
	float x = vec.x * scale;
	float y = vec.y * scale;
	return { x, y };
}

Vector gl::normalise(Vector vec)
{
	float x = vec.x / length(vec);
	float y = vec.x / length(vec);
	return { x, y };
}

double gl::length(Vector vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}
