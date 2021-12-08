#include "Game.h"

#undef main


int main()
{
	Game* game = new Game();
	delete game;
	return 0;
}