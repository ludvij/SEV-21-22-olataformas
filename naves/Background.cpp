#include "Background.h"


Background::Background(std::string filename, float x, float y, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) 
{
}

void Background::draw(float scrollX, float scrollY)
{
	Actor::draw();
}
