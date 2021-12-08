#include "MenuLayer.h"

#include <iostream>


MenuLayer::MenuLayer(Game* game)
	: Layer(game) 
{
	init();
}

MenuLayer::~MenuLayer()
{
	if (controller) SDL_GameControllerClose(controller);
}

void MenuLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/menu_fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	button = new Actor("res/boton_jugar.png", WIDTH * 0.5, HEIGHT * 0.7, 232, 72, game);
}

void MenuLayer::draw() {
	background->draw();
	button->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}


void MenuLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			controller = SDL_GameControllerOpen(0);
			if (controller == nullptr) {
				std::cout << "error en GamePad" << std::endl;
			}
			else {
				std::cout << "GamePad conectado" << std::endl;
			}
		}
		// Cambio automático de input
		switch (event.type) {
		case SDL_CONTROLLERBUTTONDOWN: [[fallthrough]]; // FALLTHROUGH (usar c++17 si no no compila)
		case SDL_CONTROLLERAXISMOTION:
			game->input = Input::CONTROLLER;
			break;
		case SDL_KEYDOWN:
			game->input = Input::KEYBOARD;
			break;
		case SDL_MOUSEBUTTONDOWN:
			game->input = Input::MOUSE;
			break;
		}
		// Procesar teclas
		if (game->input == Input::CONTROLLER) {  // gamePAD
			gamePadToControls(event);
		}
		if (game->input == Input::KEYBOARD) {
			keysToControls(event);
		}
		if (game->input == Input::MOUSE) {
			mouseToControls(event);
		}
	}

	//procesar controles, solo tiene uno
	if (controlContinue) {
		// Cambia la capa
		game->layer = game->gameLayer;
		controlContinue = false;
	}
}

void MenuLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			break;
		}
	}
}


void MenuLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	int motionX = event.motion.x;// / game->scaleLower;
	int motionY = event.motion.y;// / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->containsPoint(motionX, motionY)) {
			controlContinue = true;
		}
	}
}


void MenuLayer::gamePadToControls(SDL_Event event) {
	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);

	if (buttonA) {
		controlContinue = true;
	}
}


