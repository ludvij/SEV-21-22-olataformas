#include "Enemy.h"
Enemy::Enemy(std::string sprite, std::string aMoving, std::string aDying, float x, float y, Game* game)
	: Actor(sprite, x, y, 36, 40, game) 
{
	vxIntelligence = -1;
	vx = vxIntelligence;

	Enemy::state = State::MOVING;
	
	Enemy::aMoving = new Animation(aMoving, width, height, 6, 3, true, game);
	Enemy::aDying  = new Animation(aDying,  width, height, 6, 8, false, game);

	Enemy::animation = Enemy::aMoving;
}
Enemy::~Enemy()
{
	delete aMoving;
	delete aDying;
}
int Enemy::onCollision(Player* player)
{
	if (state != State::DYING && state != State::DEAD)
		return player->loseLife();
	return -1;
}
void Enemy::update() 
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
	}
	else {
		vx = 0;
	}
}

void Enemy::impacted() {
	if (state != State::DYING) {
		state = State::DYING;
	}
}

void Enemy::draw(float scrollX, float scrollY)
{
	animation->draw(x - scrollX, y - scrollY);
}
