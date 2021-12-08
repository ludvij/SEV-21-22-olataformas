#pragma once
#include "Enemy.h"
class JumpableEnemy : public Enemy
{
public:
	JumpableEnemy(float x, float y, Game* game);

	virtual int onCollision(Player* player) override;
};

