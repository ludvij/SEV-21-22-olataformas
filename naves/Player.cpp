#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) 
{
	aIdleRight     = new Animation("res/jugador_idle_derecha.png",         width, height, 6, 8, true,  game);
	aIdleLeft      = new Animation("res/jugador_idle_izquierda.png",       width, height, 6, 8, true,  game);
	aRunningRight  = new Animation("res/jugador_corriendo_derecha.png",    width, height, 6, 8, true,  game);
	aRunningLeft   = new Animation("res/jugador_corriendo_izquierda.png",  width, height, 6, 8, true,  game);
	aShootingRight = new Animation("res/jugador_disparando_derecha.png",   width, height, 6, 4, false, game);
	aShootingLeft  = new Animation("res/jugador_disparando_izquierda.png", width, height, 6, 4, false, game);
	aJumpingRight  = new Animation("res/jugador_saltando_derecha.png",     width, height, 6, 4, true,  game);
	aJumpingLeft   = new Animation("res/jugador_saltando_izquierda.png",   width, height, 6, 4, true,  game);

	state = State::MOVING;
	orientation = Orientation::RIGHT;

	animation = aIdleRight;
}

Player::~Player()
{
	delete aIdleLeft;
	delete aIdleRight;
	delete aRunningLeft;
	delete aRunningRight;
	delete aShootingLeft;
	delete aShootingRight;
	delete aJumpingLeft;
	delete aJumpingRight;
}


Projectile* Player::shoot()
{
	if (shotTime == 0)
	{
		state = State::SHOOTING;
		shotTime = shotCadence;
		aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		aShootingRight->currentFrame = 0; //"Rebobinar" animación
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == Orientation::LEFT) {
			projectile->vx = projectile->vx * -1; // Invertir
		}
		return projectile;
	}
	return NULL;
}

void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}

	bool endAnimation = animation->update();

	if (collisionDown == true) {
		onAir = false;
	}
	else {
		onAir = true;
	}

	if (onAir && state == State::MOVING) {
		state = State::JUMPING;
	}
	if (!onAir && state == State::JUMPING) {
		state = State::MOVING;
	}

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == State::SHOOTING) {
			state = State::MOVING;
		}
	}

	// Establecer orientación
	if (vx > 0) {
		orientation = Orientation::RIGHT;
	}
	else if (vx < 0) {
		orientation = Orientation::LEFT;
	}

	// Selección de animación basada en estados
	switch (state) {
	case State::JUMPING:
		if (orientation == Orientation::RIGHT) {
			animation = aJumpingRight;
		}
		else if (orientation == Orientation::LEFT) {
			animation = aJumpingLeft;
		}
		break;
	case State::SHOOTING: 
		if (orientation == Orientation::RIGHT) {
			animation = aShootingRight;
		}
		else if (orientation == Orientation::LEFT) {
			animation = aShootingLeft;
		}	
		break;
	case State::MOVING:
		if (vx != 0) {
			if (orientation == Orientation::RIGHT) {
				animation = aRunningRight;
			}
			else if (orientation == Orientation::LEFT) {
				animation = aRunningLeft;
			}
		}
		else if (vx == 0) {
			if (orientation == Orientation::RIGHT) {
				animation = aIdleRight;
			}
			else if (orientation == Orientation::LEFT) {
				animation = aIdleLeft;
			}
		}
		break;
	}


	if (shotTime > 0) {
		shotTime--;
	}
}

void Player::jump() {
	if (!onAir) {
		vy = -16;
		onAir = true;
	}
}



void Player::moveX(float axis) {
	vx = axis * 3;
}
void Player::moveY(float axis) {
	vy = axis * 3;
}

void Player::draw(float scrollX, float scrollY) {

	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y - scrollY);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y - scrollY);
		}
	}
}


int Player::loseLife()
{
	if (invulnerableTime <= 0){
		if (lives > 0) {
			lives--;
			invulnerableTime = 100;
		}
	}
	return lives;
}