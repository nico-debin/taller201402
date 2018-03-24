#include "stdafx.h"
#include "RectanguloVista.h"

RectanguloVista::RectanguloVista(sEvento estadoEvento) : Vista(estadoEvento){
	this->tipo = RECTANGULO;
	this->lados = 4;

	//this->calcularVertices();

	Logger::getLogger()->debug( "Rectangulo Vista creado" );
}

void RectanguloVista::calcularVertices(){


	/* Ubicacion de los vertices:
	V1 ___________ V2
	  |           |
	  |___________|
	V4             V3
	*/
	/*
	// V1
	this->vertices_x[0] = this->get_x() - this->get_ancho()/2;
	this->vertices_y[0] = this->get_y() - this->get_alto()/2;

	// V2
	this->vertices_x[1] = this->get_x() + this->get_ancho()/2;
	this->vertices_y[1] = this->get_y() - this->get_alto()/2;

	// V3
	this->vertices_x[2] = this->get_x() + this->get_ancho()/2;
	this->vertices_y[2] = this->get_y() + this->get_alto()/2;

	// V4
	this->vertices_x[3] = this->get_x() - this->get_ancho()/2;
	this->vertices_y[3] = this->get_y() + this->get_alto()/2;

	Utilities::rotarVertices( this->get_rot() , this->vertices_x, this->vertices_y, this->lados, this->get_x(), this->get_y());
	*/
}

void RectanguloVista::setEstado( sEvento estadoEvento ){
	Vista::setEstado( estadoEvento );
	//this->calcularVertices();

	// Se envian vertices para el dibujo particular del paralelogramo
	for( short i = 0; i < this->lados ; i++ ){
		this->vertices_x[i] = estadoEvento.verticesX[i];
		this->vertices_y[i] = estadoEvento.verticesY[i];
	}
}

// Setea vertices con unidades logicas
void RectanguloVista::setVertices(float* x, float* y){
	/*
	for (int i = 0; i < this->lados; i++){
		this->vertices_x[i] = Utilities::unidadLogica2Pixel(x[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
		this->vertices_y[i] = Utilities::unidadLogica2Pixel(y[i], Utilities::ALTO_PX, Utilities::ALTO_UL);
	}
	*/
}

void RectanguloVista::dibujar(SDL_Renderer* renderer){
	polygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
	filledPolygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);

	// Dibuja centro x,y
	this->dibujar_centro(renderer);
}

void RectanguloVista::dibujarTextura(SDL_Renderer* renderer){}
