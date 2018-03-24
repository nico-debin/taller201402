#include "stdafx.h"
#include "Vista.h"
#include "SDLController.h"

Vista::Vista( sEvento estadoEvento ){
	this->setEstado(estadoEvento);
	this->texturaObjeto = nullptr;
}

void Vista::setEstado( sEvento estadoEvento ){
	
	this->estadoActual = estadoEvento;

	//if (estado.id != estadoEvento.id) SE PODRIA VALIDAR QUE CORRESPONDAN LOS IDS
	this->id = estadoEvento.id;
	this->alto = estadoEvento.alto;
	this->ancho = estadoEvento.ancho;
	this->rot = estadoEvento.rot;
	this->x = estadoEvento.x;
	this->y = estadoEvento.y;

	//Logger::getLogger()->debug("Calculando colores RGB de " + estadoEvento.color);
/*	int vectorRGB[3];
	Utilities::strColorHexaToRGB( estadoEvento.color,vectorRGB );
	this->color_r = vectorRGB[0];
	this->color_g = vectorRGB[1];
	this->color_b = vectorRGB[2];

	this->angulo_1 = estadoEvento.angulo_1;
	this->angulo_2 = estadoEvento.angulo_2;

	this->calcularVertices();
*/
}

void Vista::setPosition(float x, float y){
	this->x = x;
	this->y = y;
}

void Vista::dibujar_centro(SDL_Renderer* renderer){
	// Dibuja centro x,y
	//ellipseRGBA(renderer, this->x, this->y, 1, 1, 255, 255, 255, 255);
	//filledEllipseRGBA(renderer,  this->x, this->y, 1, 1, 255, 255, 255, 255);
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

//int Vista::get_color_r(){
//	return this->color_r;
//}
//
//int Vista::get_color_g(){
//	return this->color_g;
//}
//
//int Vista::get_color_b(){
//	return this->color_b;
//}
//
//float Vista::get_angulo1(){
//	return this->angulo_1;
//}
//
//float Vista::get_angulo2(){
//	return this->angulo_2;
//}

sEvento Vista::getEstado(){
	return this->estadoActual;
}

int Vista::getLados(){
	return this->lados;
}

void Vista::dibujar(SDL_Renderer* renderer){


	if( this->texturaObjeto == nullptr ){
		this->texturaObjeto = SDLController::loadTexture(this->rutaTextura,renderer);
	}

	SDL_Rect renderQuad;

/*	renderQuad.x =  this->get_x() - this->get_ancho()/2;
	renderQuad.y =  this->get_y() - this->get_alto()/2;

	renderQuad.w = this->get_ancho();
	renderQuad.h = this->get_alto();
*/
	

	renderQuad.x = this->xDeDibujo - this->anchoDeDibujo/2;
	renderQuad.y = this->yDeDibujo - this->altoDeDibujo/2;

	renderQuad.w = this->anchoDeDibujo;
	renderQuad.h = this->altoDeDibujo;

	SDL_RenderCopyEx( renderer, this->texturaObjeto, NULL, &renderQuad, -Utilities::radianes2grados(this->get_rot()), NULL,SDL_FLIP_NONE);
	//SDL_RenderCopy( renderer, this->texturaObjeto, NULL, &renderQuad);

}

void Vista::setAnchoAltoZoom(int ancho, int alto, float zoom){	
	this->cacheZoomAncho[zoom] = ancho;
	this->cacheZoomAlto[zoom] = alto;
}

void Vista::setZoomAUsar(float zoom){

	anchoDeDibujo = this->cacheZoomAncho[zoom];
	altoDeDibujo  = this->cacheZoomAlto[zoom];

}

void Vista::setXYDeDibujo(float x, float y ){
	xDeDibujo = x;
	yDeDibujo = y;
}