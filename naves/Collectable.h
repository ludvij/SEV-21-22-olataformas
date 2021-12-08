#pragma once
#include "Animation.h"



#include "Actor.h"
class Collectable : public Actor
{
public:
	Collectable(int x, int y, Game* game);

	virtual void draw(float scrollX = 0, float scrollY = 0) override;

	void update();

private:
	Animation* animation;
};

