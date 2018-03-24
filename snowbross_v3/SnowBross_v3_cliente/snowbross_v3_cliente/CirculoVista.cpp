#include "stdafx.h"
#include "CirculoVista.h"

CirculoVista::CirculoVista(sEvento estadoEvento) : Vista(estadoEvento){
	this->tipo = CIRCULO;
	this->lados = 0;

	this->rutaTextura = "imagenes/TexturaCirculo.png";

	Logger::getLogger()->debug( "Circulo Vista creado" );
}
/*
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
*/