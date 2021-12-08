#pragma once
#include "Actor.h"
class Background : public Actor
{
public:
	Background(std::string filename, float x, float y, Game* game);

	virtual void draw(float scrollX = 0, float scrollY = 0) override;
};

