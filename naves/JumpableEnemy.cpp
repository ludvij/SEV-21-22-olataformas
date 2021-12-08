#include "JumpableEnemy.h"
#include <iostream>

JumpableEnemy::JumpableEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo_rojo.png", "res/enemigo_rojo_movimiento.png", "res/enemigo_rojo_morir.png", x, y, game)
{
}

int JumpableEnemy::onCollision(Player* player)
{
	int topY   = y - height/ 2;
	int leftX  = x - width / 2;
	int rightX = x + width / 2;
	if (state != State::DYING && state != State::DEAD) {
		int plBotY = player->y + player->height/ 2;
		if (player->vy > 0 && abs(topY - plBotY) < height / 2) {		
			impacted();
		}
		else {
			return player->loseLife();
		}
	}
	return -1;
}
