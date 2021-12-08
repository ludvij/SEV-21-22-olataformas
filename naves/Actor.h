#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(std::string filename, float x, float y, int width, int height, Game* game);
	~Actor();

	virtual void draw(float scrollX = 0, float scrollY = 0);

	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0, float scrollY = 0);

	bool containsPoint(int x, int y);

	bool affectedByGravity = true;

	// position
	int x;
	int y;
	// velocity in x and y coordinates
	float vx = 0.0f;
	float vy = 0.0f;
	// texture size ingame
	int width;
	int height;
	// texture size in file


	bool collisionDown = false;

	bool clicked = false;

	bool deleted = false;

protected:
	Game* game; // referencia al juego

private:
	int fileWidth;
	int fileHeight;
	SDL_Texture* texture;
};
