#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"
#include "Space.h"
#include "Pad.h"
#include "Collectable.h"

#include <list>


class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	~GameLayer();

	void init()            override;
	void draw()            override;
	void update()          override;
	void processControls() override;
	

private:
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);
	void gamepadToControls(SDL_Event event);
	
	void loadMap(std::string name);
	void loadMapObject(char character, float x, float y);

	void calculateScroll();

	void updateCollisions();
	void deleteOutOfBounds();

	template <typename T>
	void deleteActors(std::list<T>& list);


	void deleteAll();

private:

	Space* space;
	SDL_GameController* controller;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundCollectables;
	Actor* buttonJump;
	Actor* buttonShoot;
	Tile* cup;
	Pad* pad;

	std::list<Tile*>        tiles;
	std::list<Enemy*>       enemies;
	std::list<Projectile*>  projectiles;
	std::list<Collectable*> collectables;

	Text* textPoints;
	Text* textCollectables;

	float scrollX = 0;
	float scrollY = 0;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;


	int mapWidth;
	int points;
	int points_collectables;
};
