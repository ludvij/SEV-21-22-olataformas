
#include <iostream>
#include <sstream>
#include <fstream>

#include "GameLayer.h"
#include "Utility.h"
#include "BasicEnemy.h"
#include "JumpableEnemy.h"
#include "FlyingEnemy.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) 
{
	//llama al constructor del padre : Layer(renderer)
	controller = SDL_GameControllerOpen(0);
	init();
}

GameLayer::~GameLayer()
{
	deleteAll();
	if (controller) SDL_GameControllerClose(controller);
}

void GameLayer::init() 
{
	// borrar todos los punteros
	deleteAll();

	space = new Space(1);
	scrollX = 0;
	tiles.clear();
	points = 0;
	points_collectables = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = std::to_string(points);
	textCollectables = new Text("recolectables", WIDTH * 0.92, HEIGHT * 0.12, game);
	textCollectables->content = std::to_string(points_collectables);

	background = new Background("res/fondo_2.png", (WIDTH * 0.5f), (HEIGHT * 0.5f), game);
	backgroundPoints       = new Actor("res/icono_puntos.png",       WIDTH * 0.85, HEIGHT * 0.04, 24, 24, game);
	backgroundCollectables = new Actor("res/icono_recolectable.png", WIDTH * 0.85, HEIGHT * 0.12, 24, 24, game);


	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	buttonJump  = new Actor("res/boton_salto.png",   WIDTH * 0.9,  HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);


	projectiles.clear();

	enemies.clear(); // Vaciar por si reiniciamos el juego
	
	points = 0;

	loadMap("res/"+std::to_string(game->currentLevel)+".txt");
}

void GameLayer::processControls() 
{
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			controller = SDL_GameControllerOpen(0);
			if (controller == nullptr) {
				std::cout << game->frame << "\terror en GamePad" << std::endl;
			}
			else {
				std::cout << game->frame << "\tGamepad conectado" << std::endl;
			}
		}

		// Cambio automático de input
		switch (event.type) {
		case SDL_CONTROLLERBUTTONDOWN: 
			[[fallthrough]];
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

		if (game->input == Input::CONTROLLER) {  
			gamepadToControls(event);
		}
		if (game->input == Input::KEYBOARD) {
			keysToControls(event);
		}
		if (game->input == Input::MOUSE) {
			mouseToControls(event);
		}


		if (event.type == SDL_QUIT) game->loopActive = false;
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
		auto p = player->shoot();
		if (p != nullptr) {
			projectiles.push_back(p);
			space->addDynamicActor(p);
			controlShoot = false;
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
	
	}
	else if (controlMoveY < 0) {
		player->jump();
	}
	else {

	}
}

void GameLayer::update() 
{
	textPoints->content = std::to_string(points);
	textCollectables->content = std::to_string(points_collectables);
	// nivel superado
	if (cup->isOverlap(player)) {
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		init();
	}

	if (player->y > HEIGHT + 80) {
		init();
	}

	space->update();
	player->update();
	std::cout << game->frame << "\tupdate succesful" << std::endl;

	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& p : projectiles) {
		p->update();
	}
	for (const auto& collectable : collectables) {
		collectable->update();
	}

	// Colisiones
	updateCollisions();
	// delete out of bounds
	deleteOutOfBounds();
}

void GameLayer::draw() 
{
	calculateScroll();

	background->draw();
	for (const auto& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (const auto& collectable : collectables) {
		collectable->draw(scrollX, scrollY);
	}
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}
	cup->draw(scrollX, scrollY);
	player->draw(scrollX, scrollY);
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}
	textPoints->draw();
	textCollectables->draw();
	backgroundPoints->draw();
	backgroundCollectables->draw();

	// HUD
	if (game->input == Input::MOUSE) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}



	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}
}


void GameLayer::gamepadToControls(SDL_Event event) {

	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
	// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
	// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
	std::cout << game->frame << "\tbotones:" << buttonA << "," << buttonB << std::endl;
	int stickX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
	std::cout << game->frame << "\tstickX" << stickX << std::endl;

	// Retorna aproximadamente entre [-32800, 32800], el centro debería estar en 0
	// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
	if (stickX > 4000) {
		controlMoveX = 1;
	}
	else if (stickX < -4000) {
		controlMoveX = -1;
	}
	else {
		controlMoveX = 0;
	}

	if (buttonA) {
		controlShoot = true;
	}
	else {
		controlShoot = false;
	}

	if (buttonB) {
		controlMoveY = -1; // Saltar
	}
	else {
		controlMoveY = 0;
	}
}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	int motionX = event.motion.x; /// game->scaleLower;
	int motionY = event.motion.y; /// game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}

		}
		else {
			pad->clicked = false; // han sacado el ratón del pad
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}


void GameLayer::updateCollisions()
{
	for (const auto& collectable : collectables) {
		if (player->isOverlap(collectable))
		{
			collectable->deleted = true;
			points_collectables++;
		}
	}

	// iterate through all enemies and check for collisions with either the 
	// player or the projectiles
	// check if projectile is out of bounds
	for (auto const& projectile : projectiles)
	{
		if (!projectile->isInRender(scrollX) || projectile->vx == 0) {
			projectile->deleted = true;
		}
	}
	for (auto const& enemy : enemies)
	{
		if (player->isOverlap(enemy) && enemy->state != State::DYING && enemy->state != State::DEAD)
		{
			if (enemy->onCollision(player) == 0) {
				init();
				return; // Cortar el for
			}
		}
		// Colisiones , Enemy - Projectile 
		for (auto const& projectile : projectiles)
		{
			if (enemy->isOverlap(projectile) && enemy->state != State::DYING && enemy->state != State::DEAD)
			{
				projectile->deleted = true;

				enemy->impacted();

				points++;
				break;
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == State::DEAD) {
			enemy->deleted = true;
		}
	}

	deleteActors(projectiles);
	deleteActors(enemies);
	deleteActors(collectables);
}

void GameLayer::deleteOutOfBounds()
{
	for (auto const& e : enemies) {
		if (e->x <= 0) e->deleted = true;
	}
	deleteActors(enemies);
}

void GameLayer::deleteAll()
{
	delete cup;
	delete pad;
	delete space;
	delete player;
	delete background;
	delete buttonJump;
	delete textPoints;
	delete buttonShoot;
	delete backgroundPoints;
	delete textCollectables;
	delete backgroundCollectables;

	
	while (!tiles.empty())        delete tiles.back(),        tiles.pop_back();
	while (!enemies.empty())      delete enemies.back(),      enemies.pop_back();
	while (!projectiles.empty())  delete projectiles.back(),  projectiles.pop_back();
	while (!collectables.empty()) delete collectables.back(), collectables.pop_back();
}


template <typename T>
void GameLayer::deleteActors(std::list<T>& actors)
{
	auto itr = actors.begin();
	while (itr != actors.end())
	{
		T actor = *itr;
		if (actor->deleted == true) 
		{
			space->removeDynamicActor(actor);
			actors.erase(itr++);
			delete actor;
		}
		else {
			itr++;
		}
	}
}

void GameLayer::loadMap(std::string name) {
	char character = 0;
	std::string line;
	std::ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		std::cout << game->frame << "\tFalla abrir el fichero de mapa" << std::endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			std::istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				//std::cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

			//std::cout << character << std::endl;
		}
	}
	streamFile.close();
}


void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
		case '@': {
			auto collectable = new Collectable(x, y, game);
			collectable->y -= collectable->height / 2;
			collectables.push_back(collectable);
			
			break;
		}
		case 'C': {
			cup = new Tile("res/copa.png", x, y, game);
			cup->y = cup->y - cup->height / 2;
			space->addDynamicActor(cup);
			break;
		}
		case '1': {

			player = new Player(x, y, game);
			// modificación para empezar a contar desde el suelo.
			player->y = player->y - player->height / 2;
			space->addDynamicActor(player);
			break;
		}
		case '#': {
			auto tile = new Tile("res/bloque_tierra.png", x, y, game);
			// modificación para empezar a contar desde el suelo.
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'E': {
			auto enemy = new BasicEnemy(x, y, game);
			enemy->y = enemy->y - enemy->height / 2;
			enemies.emplace_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
		case 'J': {
			auto enemy = new JumpableEnemy(x, y, game);
			enemy->y = enemy->y - enemy->height / 2;
			enemies.emplace_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
		case 'F': {
			auto enemy = new FlyingEnemy(x, y, game);
			enemy->y = enemy->y - enemy->height / 2;
			enemies.emplace_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
	}
}

void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}

	// limite inferior
	scrollY = player->y - HEIGHT * 0.5f;
}

