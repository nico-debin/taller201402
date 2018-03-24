#include "stdafx.h"
#include "Vista.h"
#include "SDLController.h"

Vista::Vista( sEvento estadoEvento ){
	this->setEstado(estadoEvento);
	this->texturaObjeto = nullptr;
	this->textura = nullptr;
}

void Vista::setEstado( sEvento estadoEvento ){
	//if (estado.id != estadoEvento.id) SE PODRIA VALIDAR QUE CORRESPONDAN LOS IDS
	this->id = estadoEvento.id;
	this->alto = estadoEvento.alto;
	this->ancho = estadoEvento.ancho;
	this->rot = estadoEvento.rot;
	this->x = estadoEvento.x;
	this->y = estadoEvento.y;

	//Logger::getLogger()->debug("Calculando colores RGB de " + estadoEvento.color);
	int vectorRGB[3];
	Utilities::strColorHexaToRGB( estadoEvento.color,vectorRGB );
	this->color_r = vectorRGB[0];
	this->color_g = vectorRGB[1];
	this->color_b = vectorRGB[2];

	this->angulo_1 = estadoEvento.angulo_1;
	this->angulo_2 = estadoEvento.angulo_2;

	this->calcularVertices();
}

void Vista::setPosition(float x, float y){
	this->x = x;
	this->y = y;
}

void Vista::dibujar_centro(SDL_Renderer* renderer){
	// Dibuja centro x,y
	ellipseRGBA(renderer, this->x, this->y, 1, 1, 255, 255, 255, 255);
	filledEllipseRGBA(renderer,  this->x, this->y, 1, 1, 255, 255, 255, 255);
}

int Vista::getId(){
	return this->id;
}

float Vista::get_x(){
	return this->x;
}

float Vista::get_y(){
	return this->y;
}

float Vista::get_ancho(){
	return this->ancho;
}

float Vista::get_alto(){
	return this->alto;
}

float Vista::get_rot(){
	return this->rot;
}

int Vista::get_color_r(){
	return this->color_r;
}

int Vista::get_color_g(){
	return this->color_g;
}

int Vista::get_color_b(){
	return this->color_b;
}

float Vista::get_angulo1(){
	return this->angulo_1;
}

float Vista::get_angulo2(){
	return this->angulo_2;
}