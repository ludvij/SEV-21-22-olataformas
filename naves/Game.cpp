#include <iostream>

#include <SDL_image.h>

#include "Game.h"
#include "GameLayer.h"
#include "MenuLayer.h"



Game::Game() 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error SDL_Init" << SDL_GetError() << std::endl;
	}
	if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0) {
		std::cout << "Error Window y Renderer" << SDL_GetError() << std::endl;
	}
	SDL_SetWindowTitle(window, "Juego de Naves");
	// Escalado de imágenes de calidad
	// https://wiki.libsdl.org/SDL_HINT_RENDER_SCALE_QUALITY
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	menuLayer = new MenuLayer(this);
	gameLayer = new GameLayer(this);
	layer = menuLayer;

	TTF_Init();
	font = TTF_OpenFont("res/sans.ttf", 24);

	loopActive = true; // bucle activo
	loop();
}

SDL_Texture* Game::getTexture(std::string filename)
{
	if (mapTexture.find(filename) != mapTexture.end()) {
		std::cout << frame << '\t' << "retorno recurso cacheado: " << filename << std::endl;
	}
	else {
		std::cout << frame << '\t' << "Nuevo hay que cachearlo " << filename << std::endl;
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (surface == nullptr)
		{
			std::cout << frame << '\t' << "Archivo no encontrado " << filename << std::endl;
			SDL_FreeSurface(surface);
			return nullptr;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		mapTexture[filename] = texture;

		SDL_FreeSurface(surface);
	}

	return mapTexture[filename];
}

Game::~Game()
{
	// borrar texturas
	for (const auto& [k, v] : mapTexture) SDL_DestroyTexture(v);
	
	
	delete menuLayer;
	delete gameLayer;
	TTF_CloseFont(font);
	SDL_Quit();
}


long Game::getDeltaTime() { return difference; }
long Game::getTime() { return SDL_GetTicks(); }

void Game::loop() {

	while (loopActive) {
		start = SDL_GetTicks();

		// Controles
		layer->processControls();
		// Actualizar elementos
		layer->update();
		// Dibujar
		layer->draw();

		end = SDL_GetTicks();
		difference = end - start;
		if (difference < (1000 / 30)) {
			SDL_Delay((1000 / 30) - difference);
		}

		frame++;
	}
}