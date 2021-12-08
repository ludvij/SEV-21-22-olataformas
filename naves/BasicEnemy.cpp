#include "BasicEnemy.h"


BasicEnemy::BasicEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo.png", "res/enemigo_movimiento.png", "res/enemigo_morir.png", x, y, game)
{

}

