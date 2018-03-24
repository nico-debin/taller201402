#include "stdafx.h"
#include "CirculoVista.h"

CirculoVista::CirculoVista(sEvento estadoEvento) : Vista(estadoEvento){
	this->tipo = CIRCULO;
	this->lados = 0;
	Logger::getLogger()->debug( "Circulo Vista creado" );
	degrees = 0;
}

void CirculoVista::dibujar(SDL_Renderer* renderer){
	float radio_horizontal = this->get_ancho()/2;
	float radio_vertical = this->get_alto()/2;


	// Dibuja el ciculo
	ellipseRGBA(renderer, this->x, this->y, radio_horizontal, radio_vertical, this->color_r, this->color_g, this->color_b, 255);
	filledEllipseRGBA(renderer,  this->x, this->y, radio_horizontal, radio_vertical, this->color_r, this->color_g, this->color_b, 255);


	// Calculos para dibujar el diametro del circulo como una recta blanca
	int dx = cos(this->get_rot()*-1) * radio_horizontal;
	int dy = sin(this->get_rot()*-1) * radio_horizontal;
	
	int x1 = this->get_x() - dx;
	int y1 = this->get_y() - dy;

	int x2 = this->get_x() + dx;
	int y2 = this->get_y() + dy;

	// Dibuja el diametro
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void CirculoVista::dibujarTextura(SDL_Renderer* renderer){

	if(this->textura == nullptr){
		this->textura=SDLController::loadTexture("imagenes/Circulo3.png",renderer);		
		if (this->textura==nullptr){
			Logger::getLogger()->error("Se utiliza la imagen de fondo por default: "+(string)IMAGEN_FONDO_DEFAULT);
			this->textura = SDLController::loadTexture(IMAGEN_FONDO_DEFAULT, renderer);
		}
	}

	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	SDL_Rect renderQuad = { this->x - this->get_ancho()/2,this->y - this->get_ancho()/2, this->get_ancho(), this->get_alto()};

	degrees += 10;

	SDL_Rect clip={0,0,390,388}; 
	SDL_RenderCopyEx( renderer, this->textura, &clip, &renderQuad, degrees,NULL, flipType );

}

