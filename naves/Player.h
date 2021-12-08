#pragma once
#include "Actor.h"
#include "Projectile.h"
#include "Animation.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	~Player();

	Projectile* shoot();
	void update();
	void jump();
	void moveX(float axis);
	void moveY(float axis);


	int loseLife();



	virtual void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir

	Orientation orientation;
	State state;


private:

	int lives = 3;
	int invulnerableTime = 0;

	int shotCadence = 30;
	int shotTime = 0;

	bool onAir = false;

	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* aJumpingRight;
	Animation* aJumpingLeft;

	Animation* animation; // Referencia a la animación mostrada

};

