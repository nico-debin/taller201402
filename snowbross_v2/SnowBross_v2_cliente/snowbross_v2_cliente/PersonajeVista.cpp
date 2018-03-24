#include "StdAfx.h"
#include "PersonajeVista.h"
#include "Logger.h"
#include "Constantes.h"

PersonajeVista::PersonajeVista(sEvento estadoEvento): Vista(estadoEvento){
	//this->estadoPersonaje = CREADO;
	//this->estadoPersonaje = QUIETO;
	this->estadoPersonaje = EVENTO_CLIENTE_PERSONAJE_QUIETO;
	this->flip = SDL_FLIP_NONE;
    this->spriteTexture = nullptr;
    this->useClip = 0;
	this->actualClip = 0;
}

PersonajeVista::~PersonajeVista(){
	for( std::map< int,sSprite*>::iterator i = this->SpriteMap.begin(); i != this->SpriteMap.end(); ) {
		delete i->second;
		SpriteMap.erase( i++ ); //borro y avanzo el iterador
	}
}

void PersonajeVista::dibujar(SDL_Renderer* renderer){

	// Creacion de sprite
	if( this->spriteTexture == nullptr ){
		this->spriteTexture = SDLController::loadTexture(this->spriteSheetName,renderer);
	}

	if( SpriteMap.empty() ){
		this->inicializacionEstados();
	}

	//if ( !ControladorCliente::juegoEnPausa ){
	if ( !Utilities::PAUSA_CLIENTE  ){
		if(this->useClip < PERSONAJE_VISTA_CLIP_COUNT){
			this->actualClip = 0;
		}else if(this->useClip < PERSONAJE_VISTA_CLIP_COUNT*2){
			this->actualClip = 1;
		}else if(this->useClip < PERSONAJE_VISTA_CLIP_COUNT*3 -1 ){
			this->actualClip = 2;
		}else{
			this->actualClip = 2;
			this->useClip = -1;
		}
		this->useClip++;
	}
	SDL_Rect renderQuad;

	//int altoPx = this->get_alto();
	//int anchoPx = this->get_ancho();

   /* renderQuad.x =  this->get_x() - anchoPx/2;

    int yCentrado = this->get_y();        
    renderQuad.y =  (yCentrado - altoPx/2);*/

	sSprite*  spriteClips =  this->SpriteMap[getEstadoPersonaje()];	 
	//renderQuad.w = anchoPx;
	//renderQuad.h = altoPx;
	
	renderQuad.x = this->xDeDibujo - this->anchoDeDibujo/2;
	renderQuad.y = this->yDeDibujo - this->altoDeDibujo/2;

	renderQuad.w = this->anchoDeDibujo;
	renderQuad.h = this->altoDeDibujo;


	SDL_RenderCopyEx( renderer, this->spriteTexture, &spriteClips->sprites[this->actualClip], &renderQuad,0.0,NULL,this->flip); 		
}

int PersonajeVista::getEstadoPersonaje(){
	return this->estadoPersonaje;
}

void PersonajeVista::setEstadoPersonaje( int nuevoEstado ){
	 this->estadoPersonaje = nuevoEstado;
}

void PersonajeVista::setEstado( sEvento estadoEvento ){
	//if ( this->estadoPersonaje == MURIENDO) return;
	//if ( this->estadoPersonaje == CREADO  ) return;

	if(( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER ) || ( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER )){
		   this->flip = SDL_FLIP_HORIZONTAL;
	}
	if(( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ ) || ( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ )){
		   this->flip = SDL_FLIP_NONE;
	}


	this->estadoPersonaje = estadoEvento.evento;
	

	//if (  this->get_x() < estadoEvento.x && this->get_y() == estadoEvento.y) {
	//  //Cambio de sentido el sprite
	//  this->flip = SDL_FLIP_HORIZONTAL;
	//  this->estadoPersonaje = CAMINANDO;
	//} else if ( this->get_x() > estadoEvento.x && this->get_y() == estadoEvento.y) {
	//	     // No cambio de sentido el sprite
	//	     this->flip = SDL_FLIP_NONE;
	//		  this->estadoPersonaje = CAMINANDO;
	//	   } else if ( this->get_y() == estadoEvento.y ) {
	//				this->estadoPersonaje = QUIETO;
	//			  } else if(  this->get_y() > estadoEvento.y ){
	//						this->estadoPersonaje = SALTANDO;
	//					 }

	Vista::setEstado( estadoEvento );
}











