#pragma once
#include "Game.h"


class Animation
{
public:
	Animation(std::string filename, int actorWidth, int actorHeight,
		int updateFrecuence, int totalFrames, bool loop, Game* game);
	~Animation();
	bool update();
	void draw(int x, int y);

	int totalFrames;
	int currentFrame;
	bool loop;
	int updateFrecuence = 0;
	int updateTime = 0;

protected:

	Game* game;

private:

	int actorWidth; // Actor Width
	int actorHeight; // Actor height
	int fileWidth; // Sprite Sheet width
	int fileHeight; // Sprite Sheet height
	// Auxiliares
	SDL_Texture* texture; // imagen
	SDL_Rect source; // Parte de la imagen que se recorta del fichero
	int frameWidth; // Ancho de un fotograma
	int frameHeigt; // Alto de un fotograma
};
