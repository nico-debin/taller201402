#include "StdAfx.h"
#include "Dragon.h"
#include "Fuego.h"

Dragon::Dragon(b2World* mundo,  sObjeto structObj): Enemigo(mundo, structObj){
	this->userData.tipo = TIPO_DRAGON;
	this->timerFuego = nullptr;
	this->timerQuieto = nullptr;
}

Dragon::~Dragon(){
	delete this->timerFuego;
}

void Dragon::analizarLanzarFuego(){
	if( this->timerQuieto == nullptr ){
		this->timerQuieto = new Timer();
		this->timerQuieto->start();
	}

	if ( this->timerFuego == nullptr ){
		this->timerFuego = new Timer();
		this->timerFuego->start();
	} 
	
	if(( this->getEstado() == EVENTO_CLIENTE_PERSONAJE_QUIETO     ||
		 this->getEstado() == EVENTO_CLIENTE_PERSONAJE_QUIETO_DER ||
		 this->getEstado() == EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ    ) && this->timerQuieto->getTicks() > 2000 ){
			this->timerQuieto->stop();
			this->timerQuieto->start();
		if( this->datos.id%2 == 0 ){ 
			this->moverIzquierda();
		} else {
			this->moverDerecha();
		}
	} else if( this->timerFuego->getTicks() > DRAGON_TIEMPO_LANZAR_FUEGO ) {
		if ( this->getEstado() ==  EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER ){
			this->frenarDerecha();			
			this->timerFuego->stop();
			this->lanzar();	
			this->timerFuego->start();
		} else if ( this->getEstado() ==  EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ ) {
			this->frenarIzquierda();
			this->timerFuego->stop();
			this->lanzar();	
			this->timerFuego->start();
		}	
	}
}

Figura* Dragon::crearProyectil(sObjeto structDelProyectil){
	return new Fuego(this->body->GetWorld(), structDelProyectil);
}