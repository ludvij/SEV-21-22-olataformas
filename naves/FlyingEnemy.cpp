#include "FlyingEnemy.h"
#include <cmath>

FlyingEnemy::FlyingEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo_azul.png", "res/enemigo_azul_movimiento.png", "res/enemigo_azul_morir.png", x, y, game)
{
	affectedByGravity = false;
	vyIntelligence = 1;
	vy = vyIntelligence;
}

void FlyingEnemy::update()
{
	bool endAnimation = animation->update();
	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == State::DYING) {
			state = State::DEAD;
		}
	}

	if (state == State::MOVING) {
		animation = aMoving;
		vy = 5 * sinf(game->frame * 0.1 * M_PI);
	}
	if (state == State::DYING) {
		animation = aDying;
	}
	// Establecer velocidad
	if (state != State::DYING) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (vy == 0) {
			vyIntelligence = vyIntelligence * -1;
		}
	}
	else {
		vx = 0;
		vy = 0;
	}
}
