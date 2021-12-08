#pragma once
#include "Enemy.h"
class FlyingEnemy : public Enemy
{
public:
	FlyingEnemy(float x, float y, Game* game);

	virtual void update() override;

protected:
	float vyIntelligence;
};

