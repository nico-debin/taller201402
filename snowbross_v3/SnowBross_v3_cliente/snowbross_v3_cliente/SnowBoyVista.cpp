#include "StdAfx.h"
#include "SnowBoyVista.h"

SnowBoyVista::SnowBoyVista(sEvento estadoEvento): PersonajeVista(estadoEvento){
	this->spriteSheetName = "imagenes/SnowBrosSheet1.gif";
	this->tipo = SNOWBOY;

	this->estadoPersonajeReal = EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ;
}

void SnowBoyVista::inicializacionEstados(){
	// INICIALIZO PARAMETROS GLOBALES SPRITE
	int x_start = 206; 
	int y_start = 2;
	int iW = SNOWBOY_ANCHO_PX; 
	int iH = SNOWBOY_ALTO_PX;
	
	int iSpriteInicial = 0;

	// SPRITE CREAR SNOWBOY
	sSprite* spriteCrear = new sSprite();
	spriteCrear->size = 3;
	for (int i = 0; i < 3; ++i){
		spriteCrear->sprites[i].x = x_start + (i+iSpriteInicial)*iW;
		spriteCrear->sprites[i].y = y_start;
		spriteCrear->sprites[i].w = iW;
		spriteCrear->sprites[i].h = iH;
	}
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CREADO] = spriteCrear;

	// SPRITE QUIETO
	x_start = 30;
    y_start = 44;

	sSprite* spriteQuieto = new sSprite();
	spriteQuieto->size = 1;
	spriteQuieto->sprites[0].x = 32;
	spriteQuieto->sprites[0].y = 44;
	spriteQuieto->sprites[0].w = 18;
	spriteQuieto->sprites[0].h = 26;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO] = spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ] = spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO_DER] = spriteQuieto;
    
	// SPRITE CAMINANDO
    x_start = 55; 
	y_start = 44;
	iSpriteInicial = 0;

	sSprite* spriteCaminando = new sSprite();
	spriteCaminando->size = 3;

	spriteCaminando->sprites[0].x = 55;
	spriteCaminando->sprites[0].y = 44;
	spriteCaminando->sprites[0].w = 17;
	spriteCaminando->sprites[0].h = 26;

	spriteCaminando->sprites[1].x = 77;
	spriteCaminando->sprites[1].y = 44;
	spriteCaminando->sprites[1].w = 17;
	spriteCaminando->sprites[1].h = 26;

	spriteCaminando->sprites[2].x = 98;
	spriteCaminando->sprites[2].y = 44;
	spriteCaminando->sprites[2].w = 17;
	spriteCaminando->sprites[2].h = 26;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ] = spriteCaminando ;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER] = spriteCaminando ;

	// SPRITE SALTANDO
	x_start = 55; 
	y_start = 43;
	iSpriteInicial = 5;
	sSprite* spriteSaltando = new sSprite();
	spriteSaltando->size = 3;

	spriteSaltando->sprites[0].x = 146;
	spriteSaltando->sprites[0].y = 47;
	spriteSaltando->sprites[0].w = 18;
	spriteSaltando->sprites[0].h = 28;

	spriteSaltando->sprites[1].x = 165;
	spriteSaltando->sprites[1].y = 42;
	spriteSaltando->sprites[1].w = 18;
	spriteSaltando->sprites[1].h = 28;

	spriteSaltando->sprites[2].x = 188;
	spriteSaltando->sprites[2].y = 46;
	spriteSaltando->sprites[2].w = 18;
	spriteSaltando->sprites[2].h = 28;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO] = spriteSaltando;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ] = spriteSaltando;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER] = spriteSaltando;

	
	// SPRITE MURIENDO
	x_start = 134;
	y_start = 103;

	iW = 31; 
    iH = 32;

	iSpriteInicial = 0;
	sSprite* spriteMuriendo = new sSprite();
	spriteMuriendo->size = 3;
	for (int i = 0; i < 3; ++i){
		spriteMuriendo->sprites[i].x = x_start + (i+iSpriteInicial)*iW;
		spriteMuriendo->sprites[i].y = y_start;
		spriteMuriendo->sprites[i].w = iW;
		spriteMuriendo->sprites[i].h = iH;
	}
	this->SpriteMap[EVENTO_CLIENTE_FIGURA_MURIENDO] = spriteMuriendo;

	sSprite* spriteDesconectado = new sSprite();
	spriteDesconectado->size = 1;
	spriteDesconectado->sprites[0].x = 25;
	spriteDesconectado->sprites[0].y = 486;
	spriteDesconectado->sprites[0].w = 18;
	spriteDesconectado->sprites[0].h = 24;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_DESCONECTADO] = spriteDesconectado;



	// SPRITE CORRIENDO
	iW = 18; 
    iH = 25;

	iSpriteInicial = 0;

	sSprite* spriteCorriendo = new sSprite();
	spriteCorriendo->size = 2;
	spriteCorriendo->sprites[0].x = 118;
	spriteCorriendo->sprites[0].y = 75;
	spriteCorriendo->sprites[0].w = iW;
	spriteCorriendo->sprites[0].h = iH;

	spriteCorriendo->sprites[1].x = 142;
	spriteCorriendo->sprites[1].y = 75;
	spriteCorriendo->sprites[1].w = iW;
	spriteCorriendo->sprites[1].h = iH;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CORRIENDO_DER] = spriteCorriendo;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CORRIENDO_IZQ] = spriteCorriendo;

	// SPRITE CORRIENDO QUIETO

	iW = 19; 
    iH = 26;

	iSpriteInicial = 0;

	sSprite* spriteCorriendoQuieto = new sSprite();
	spriteCorriendoQuieto->size = 2;
	spriteCorriendoQuieto->sprites[0].x = 67;
	spriteCorriendoQuieto->sprites[0].y = 74;
	spriteCorriendoQuieto->sprites[0].w = iW;
	spriteCorriendoQuieto->sprites[0].h = iH;

	spriteCorriendoQuieto->sprites[1].x = 93;
	spriteCorriendoQuieto->sprites[1].y = 74;
	spriteCorriendoQuieto->sprites[1].w = iW;
	spriteCorriendoQuieto->sprites[1].h = iH;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER] = spriteCorriendoQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ] = spriteCorriendoQuieto;
}


void SnowBoyVista::dibujar( SDL_Renderer* renderer ){
	if(this->timerDesconectado.isStarted() && this->timerDesconectado.getTicks() > 1000){
		// Si estuvo gris más de 5 cinco segundos, ya no lo dibujo más.
		return;
	}
	if( this->estadoPersonaje != EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE &&
	    this->estadoPersonaje != EVENTO_CLIENTE_SNOWBOY_INVENCIBLE &&
		this->estadoPersonaje != EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER)  {
		PersonajeVista::dibujar(renderer);
	}
}

void SnowBoyVista::setEstado( sEvento estadoEvento ){

	//ControlSonido controlSonido;
	
	bool salto =false;
	//Reproducir sonido de salto
	if ( ( this->estadoPersonajeReal != EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER && this->estadoPersonajeReal != EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ )  &&  ( estadoEvento.evento == EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER  || estadoEvento.evento == EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ )  ){
		//controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::SALTO);
		this->controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::SALTO);
		salto =true;
	}
	else if ( ( this->estadoPersonajeReal == EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER || this->estadoPersonajeReal == EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ )  &&  ( estadoEvento.evento != EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER  && estadoEvento.evento != EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ )  ){
		//controlSonido.tocar(ControlRemoto::SINSONIDO, Temas::SALTO);
	}




	//Reproducir sonido muriendo
	if ( ( this->estadoPersonajeReal != EVENTO_CLIENTE_FIGURA_MURIENDO ) && ( estadoEvento.evento == EVENTO_CLIENTE_FIGURA_MURIENDO ) ){
		controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MURIENDO);
	}
	else if ( ( this->estadoPersonajeReal == EVENTO_CLIENTE_FIGURA_MURIENDO ) && ( estadoEvento.evento != EVENTO_CLIENTE_FIGURA_MURIENDO ) ){
		//controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::MURIENDO);
		//controlSonido.tocar(ControlRemoto::SINSONIDO, Temas::MURIENDO);
	}


	if (estadoEvento.evento != EVENTO_CLIENTE_SNOWBOY_INVENCIBLE && estadoEvento.evento != EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER ){
		this->estadoPersonajeReal = estadoEvento.evento;
	}
	PersonajeVista::setEstado(estadoEvento);


	if( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_DER || this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER){
		   this->flip = SDL_FLIP_HORIZONTAL;
	}
	if( this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_IZQ || this->estadoPersonaje == EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ){
		   this->flip = SDL_FLIP_NONE;
	}


	
	/*if (salto){
		controlSonido.tocar(ControlRemoto::SINSONIDO, Temas::SALTO);
	}*/

	if(estadoEvento.evento == EVENTO_CLIENTE_PERSONAJE_DESCONECTADO){
		this->timerDesconectado.start();
	}
}