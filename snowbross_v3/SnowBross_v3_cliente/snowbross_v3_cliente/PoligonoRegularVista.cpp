#include "StdAfx.h"
#include "PoligonoRegularVista.h"


PoligonoRegularVista::PoligonoRegularVista(sEvento estadoEvento) : Vista(estadoEvento){}


void PoligonoRegularVista::calcularVertices(){
		
	//// Froma de Juanma para poligonos regulares
	//float angulo = this->rot*-1;
	//float radio = this->get_ancho() / 2;

	//Utilities::calcularVerticesPoligonoRegularVista(  this->vertices_x, this->vertices_y, this->lados ,radio, angulo );

	//// Aplico transformacino a coordenadas originales
	//for(short i = 0 ; i < this->lados ; i++){
	//	this->vertices_x[i] += this->get_x();
	//	this->vertices_y[i] += this->get_y();
	//}

}

void PoligonoRegularVista::setEstado( sEvento estadoEvento ){
	Vista::setEstado( estadoEvento );

	// Se envian vertices para el dibujo particular del paralelogramo
	//for( short i = 0; i < this->lados ; i++ ){
	//	this->vertices_x[i] = estadoEvento.verticesX[i];
	//	this->vertices_y[i] = estadoEvento.verticesY[i];
	//}
}


// Setea vertices con unidades logicas
void PoligonoRegularVista::setVertices(float* x, float* y){
	/*
	for (int i = 0; i < this->lados; i++){
		this->vertices_x[i] = Utilities::unidadLogica2Pixel(x[i], Utilities::ANCHO_PX, Utilities::ANCHO_UL);
		this->vertices_y[i] = Utilities::unidadLogica2Pixel(y[i], Utilities::ALTO_PX, Utilities::ALTO_UL);
	}
	*/
}

/*void PoligonoRegularVista::dibujar(SDL_Renderer* renderer){


	if( this->texturaObjeto == nullptr ){
		this->texturaObjeto = SDLController::loadTexture(this->rutaTextura,renderer);
	}

	SDL_Rect renderQuad;

	renderQuad.x =  this->get_x() - this->get_ancho()/2;
	renderQuad.y =  this->get_y() - this->get_alto()/2;

	renderQuad.w = this->get_ancho();
	renderQuad.h = this->get_alto();
	
	//Esto sacarlo
	if (Utilities::rotation){
		SDL_RenderCopyEx( renderer, this->texturaObjeto, NULL, &renderQuad, -Utilities::radianes2grados(this->get_rot()), NULL,SDL_FLIP_NONE);
	}
	else{SDL_RenderCopy( renderer, this->texturaObjeto, NULL, &renderQuad);}
	
	//Esto tambien
	if(Utilities::debugDraw){
		polygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);
		filledPolygonRGBA(renderer,this->vertices_x,this->vertices_y,this->lados,this->get_color_r(),this->get_color_g(),this->get_color_b(),255);

		// Dibuja centro x,y
		this->dibujar_centro(renderer);
	}
}*/


