#pragma once
#include <SDL.h>
#include <string>


class Texture {
public:
	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y);

	//Gets image dimensions
	int getWidth();
	int getHeight();
	SDL_Texture* getTexture();

private:
	SDL_Texture* pTexture;

	int pWidth;
	int pHeight;

	int pX;
	int pY;

	SDL_Rect pRenderRect;
};
