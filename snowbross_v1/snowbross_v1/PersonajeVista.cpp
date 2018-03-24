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

	int altoPx = Utilities::unidadLogica2Pixel(SNOWBOY_ALTO_UL, Utilities::ALTO_PX, Utilities::ALTO_UL);
	
    int anchoPx = Utilities::unidadLogica2Pixel(SNOWBOY_ANCHO_UL, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
    renderQuad.x =  this->get_x() - anchoPx/2;

    //Correccion por flotación, se suma por que sdl aumenta hacia abajo las coordenadas
    int yCentrado = this->get_y();        
    //yCentrado = yCentrado +  Utilities::unidadLogica2Pixel(RADIO_CUERPO/2, Utilities::ANCHO_PX, Utilities::ANCHO_UL);        
    renderQuad.y =  (yCentrado - altoPx/2);

	sSprite*  spriteClips =  this->SpriteMap[getEstadoPersonaje()];	 
	renderQuad.w = Utilities::unidadLogica2Pixel(SNOWBOY_ANCHO_UL, Utilities::ANCHO_PX, Utilities::ANCHO_UL);
	renderQuad.h = Utilities::unidadLogica2Pixel(SNOWBOY_ALTO_UL, Utilities::ALTO_PX, Utilities::ALTO_UL);

   // std::stringstream mensaje;
   // mensaje << "Clip nro. " << clip << "(x,y,w,h)=("<<renderQuad.x <<", "<< renderQuad.y <<", "<< renderQuad.w <<", "<< renderQuad.h <<")";
   // Logger::getLogger()->debug( mensaje.str() );
	
	SDL_RenderCopyEx( renderer, this->spriteTexture, &spriteClips->sprites[this->actualClip], &renderQuad,0.0,NULL,this->flip); 	
	
	// Dibuja centro x,y
	//this->dibujar_centro(renderer);
	
}


void PersonajeVista::dibujarTextura(SDL_Renderer* renderer){}

int PersonajeVista::getEstadoPersonaje(){
	return this->estadoPersonaje;
}

void PersonajeVista::setEstadoPersonaje( int nuevoEstado ){
	 this->estadoPersonaje = nuevoEstado;
}

void PersonajeVista::setEstado( sEvento estadoEvento ){
	//if ( this->estadoPersonaje == MURIENDO) return;
	//if ( this->estadoPersonaje == CREADO  ) return;
	this->estadoPersonaje = estadoEvento.evento;
		
	if ( this->get_x() < estadoEvento.x ) {
		  this->flip = SDL_FLIP_HORIZONTAL;
	}

	if ( this->get_x() > estadoEvento.x ) {
			this->flip = SDL_FLIP_NONE;
	}

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


