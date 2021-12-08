#pragma once

// C++ things
#include <string>
#include <map>

// Todas las librerias de SDL y otras utilidades
#include <SDL.h>
#include <SDL_ttf.h>

#include "Layer.h"



// Valores generales
#define WIDTH 480
#define HEIGHT 320
class Layer;


enum class Input : int {
	KEYBOARD   = 1,
	MOUSE      = 2,
	CONTROLLER = 3,
};


enum class State : int {
	MOVING  = 1,
	JUMPING = 2,
	DYING   = 3,
	DEAD    = 4,
	SHOOTING= 5,
};

enum class Orientation : int {
	LEFT  = 1,
	RIGHT = 2,
};

class Game
{
public:
	Game();
	~Game();
	void loop();
	long getDeltaTime();
	long getTime();


	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador

	TTF_Font* font;
	bool loopActive; // Juego activo

	Layer* gameLayer;
	Layer* layer;
	Layer* menuLayer;

	SDL_Texture* getTexture(std::string name);

	uint32_t frame = 0;

	int currentLevel = 0;
	int finalLevel = 2;


	Input input;


private:
	long start, end, difference;
	std::map<std::string, SDL_Texture*> mapTexture;


};

