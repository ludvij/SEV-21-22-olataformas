#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo_jugador2.png", x, y, 20, 20, game)
{
	affectedByGravity = true;
	vx = 9;
	prevVy = 0;
}

void Projectile::update() {

	if (bounces == 0) {
		deleted = true;
	}
	else if (vy == 0 && prevVy > 0) {
		vy -= 2 * bounces--;
	}
	prevVy = vy;
}