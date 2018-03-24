#include "StdAfx.h"
#include "ProyectilVista.h"


ProyectilVista::ProyectilVista(sEvento estadoEvento): Vista(estadoEvento){
	this->estadoProyectil = estadoEvento.evento;
	this->tipo = PROYECTIL;
}

void ProyectilVista::dibujar(SDL_Renderer* renderer){

	//if( this->texturaObjeto == nullptr ){
		this->texturaObjeto = SDLController::loadTexture(this->rutaTextura,renderer);
	//}

	SDL_Rect* clipElegido = this->elegirClip();

	SDL_Rect renderQuad;
	renderQuad.x = this->xDeDibujo - this->anchoDeDibujo/2;
	renderQuad.y = this->yDeDibujo - this->altoDeDibujo/2;
	renderQuad.w = this->anchoDeDibujo;
	renderQuad.h = this->altoDeDibujo;

	SDL_RenderCopy( renderer, this->texturaObjeto, clipElegido , &renderQuad);
}

SDL_Rect* ProyectilVista::elegirClip(){

	if ( this->estadoProyectil == EVENTO_CLIENTE_PROYECTIL_DER ){return &this->clipDer;}
	else if ( this->estadoProyectil == EVENTO_CLIENTE_PROYECTIL_IZQ ){return &this->clipIzq;}
	else {return nullptr;}
}


void ProyectilVista::setEstado( sEvento estadoEvento ){

	if(estadoEvento.evento != EVENTO_CLIENTE_FIGURA_MURIENDO && estadoEvento.evento != EVENTO_CLIENTE_FIGURA_MUERTO ){
		this->estadoProyectil = estadoEvento.evento;
	}
	Vista::setEstado( estadoEvento );

}




