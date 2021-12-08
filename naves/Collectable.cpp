#include "Collectable.h"

Collectable::Collectable(int x, int y, Game* game)
	: Actor("res/icono_recolectable.png", x, y, 35, 35, game)
{
	animation = new Animation("res/recolectable.png", this->width, this->height, 3, 8, true, this->game);
}

void Collectable::draw(float scrollX, float scrollY)
{
	animation->draw(x - scrollX, y - scrollY);
}


void Collectable::update()
{
	animation->update();
}
