#include "StdAfx.h"
#include "TrapecioVista.h"

TrapecioVista::TrapecioVista(sEvento estadoEvento) : Vista(estadoEvento){
	this->tipo = TRAPECIO;
	this->lados = LADOS_TRAPECIO;

	//this->calcularVertices();

	Logger::getLogger()->debug( "Trapecio Vista creado" );
}

void TrapecioVista::calcularVertices(){
	/*
	// Un trapecio se define por su base inferior, su alto y sus dos angulo internos inferiores (izquierdo y derecho)
	Utilities::calcularVerticesTrapecioVista( this->vertices_x, this->vertices_y, this->get_ancho(), this->get_alto(), this->get_angulo1(), this->get_angulo2(), this->get_rot() );

	// Aplico transformacino a coordenadas originales
	for(short i = 0 ; i < this->lados ; i++){
		this->vertices_x[i] += this->get_x();
		this->vertices_y[i] += this->get_y();
	}
	*/
}

void TrapecioVista::setEstado( sEvento estadoEvento ){
	Vista::setEstado( estadoEvento );
	//this->calcularVertices();

	// Se setean vertices para el dibujo particular del trapecio
	for( short i = 0; i < 4 ; i++ ){
		this->vertices_x[i] = estadoEvento.verticesX[i];
		this->vertices_y[i] = estadoEvento.verticesY[i];
	}
}


// Setea vertices con unidades logicas
void TrapecioVista::setVertices(float* x, float* y){
	for (int i = 0; i < this->lados; i++){
		this->vertices_x[i] = Utilities::unidadLogica2Pixel(x[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
		this->vertices_y[i] = Utilities::unidadLogica2Pixel(y[i], Utilities::ALTO_PX, Utilities::ALTO_UL);
	}
}

void TrapecioVista::dibujar(SDL_Renderer* renderer){
	

	if( this->texturaObjeto == nullptr ){
		this->texturaObjeto = SDLController::loadTexture("imagenes/TexturaTrapecio.png",renderer);
	}

	SDL_Rect renderQuad;

	renderQuad.x =  this->get_x() - this->get_ancho()/2;
	renderQuad.y =  this->get_y() - this->get_alto()/2;

	renderQuad.w = this->get_ancho();
	renderQuad.h = this->get_alto();
	
	//printf("radianes: %.3f, grados: %.3f    ",this->get_rot(),-Utilities::radianes2grados(this->get_rot()));


	SDL_RenderCopyEx( renderer, this->texturaObjeto, NULL, &renderQuad, -Utilities::radianes2grados(this->get_rot()), NULL,SDL_FLIP_NONE);

	if(Utilities::debugDraw){
		polygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
		filledPolygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
	}

	// Dibuja centro x,y
	this->dibujar_centro(renderer);










	// Dibuja centro x,y
	this->dibujar_centro(renderer);



}

void TrapecioVista::dibujarTextura(SDL_Renderer* renderer){}