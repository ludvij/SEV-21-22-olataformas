#include "Animation.h"

Animation::Animation(std::string filename, int actorWidth, int actorHeight,
	int updateFrecuence, int totalFrames, bool loop, Game* game) 
	: actorWidth(actorWidth) ,actorHeight(actorHeight), 
	  updateFrecuence(updateFrecuence),
	  totalFrames(totalFrames),
	  loop(loop),
	  game(game)
{
	// Cargar textura
	texture = game->getTexture(filename);
	SDL_QueryTexture(texture, nullptr, nullptr, &fileWidth, &fileHeight);
	updateTime = 0; // �ltima actualizaci�n
	currentFrame = 0;
	// Calcular lo que mide un fotograma/frame
	frameWidth = fileWidth / totalFrames;
	frameHeigt = fileHeight;
	// Rectangulo de recorte de fotograma
	source.x = 0;
	source.y = 0;
	source.w = frameWidth;
	source.h = frameHeigt;
}

Animation::~Animation()
{
}

bool Animation::update() {
	updateTime++;
	if (updateTime > updateFrecuence) {
		updateTime = 0;
		// Actualizar el frame
		currentFrame++;
		// Si lleva al ultimo frame vuelve al primero
		if (currentFrame >= totalFrames) {
			// Reiniciar es infinita
			if(loop == false) {
				// No es infinita
			   // Indicar que finaliz� 
				return true;
			}
			else {
				currentFrame = 0;
			}
		}
	}
	//Actualizar el rectangulo del source (siguiente frame)
	source.x = currentFrame * frameWidth;
	return false; // luego lo cambiamos
}

void Animation::draw(int x, int y) {
	SDL_Rect destination;
	destination.x = x - actorWidth / 2;
	destination.y = y - actorHeight / 2;
	destination.w = actorWidth;
	destination.h = actorHeight;
	// Modificar para que la referencia sea el punto central
	SDL_RenderCopyEx(game->renderer,
		texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
}

