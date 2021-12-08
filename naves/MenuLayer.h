#pragma once

#include "Layer.h"
#include "Actor.h"
#include "Background.h"

class MenuLayer : public Layer
{
public:
	MenuLayer(Game* game);
	~MenuLayer();

	virtual void init()            override;
	virtual void draw()            override;
	virtual void update()          override {};
	virtual void processControls() override;

	bool controlContinue;




private:

	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event);
	void gamePadToControls(SDL_Event event);


	SDL_GameController* controller; // Mando
	Background* background;
	Actor* button;
};
