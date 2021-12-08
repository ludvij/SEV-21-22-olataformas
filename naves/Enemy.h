#pragma once
#include "Actor.h"
#include "Animation.h"
#include "Player.h"

class Enemy : public Actor
{
public:
	Enemy(std::string sprite, std::string aMoving, std::string aDying, float x, float y, Game* game);
	~Enemy();

	virtual int onCollision(Player* player);

	virtual void update();

	virtual void draw(float scrollX = 0, float scrollY = 0) override;

	void impacted(); // Recibe impacto y pone animación de morir
	State state;

protected:
	float vxIntelligence;

	Animation* aDying;
	Animation* aMoving;
	Animation* animation;
};