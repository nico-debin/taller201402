#include "StdAfx.h"
#include "SnowBoyVista.h"

SnowBoyVista::SnowBoyVista(sEvento estadoEvento): PersonajeVista(estadoEvento){
	this->spriteSheetName = "imagenes/SnowBrosSheet1.gif";
}

void SnowBoyVista::inicializacionEstados(){
	// INICIALIZO PARAMETROS GLOBALES SPRITE
	int x_start = 206; 
	int y_start = 2;
	int iW = SNOWBOY_ANCHO_PX; 
	int iH = SNOWBOY_ALTO_PX;
	
	int iSpriteInicial = 0;

	// SPRITE CREAR SNOWBOY
	SDL_Rect clipsCrear[3];
	sSprite* spriteCrear = new sSprite();


	for (int i = 0; i < 3; ++i){
		spriteCrear->sprites[i].x = x_start + (i+iSpriteInicial)*iW;
		spriteCrear->sprites[i].y = y_start;
		spriteCrear->sprites[i].w = iW;
		spriteCrear->sprites[i].h = iH;
	}
	
	//this->SpriteMap[CREADO] = spriteCrear;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CREADO] = spriteCrear;
	// SPRITE QUIETO
	x_start = 30;
	//y_start = 40;
    y_start = 44;
	iW = SNOWBOY_ANCHO_PX; 
	iH = SNOWBOY_ALTO_PX;

	// Inicio del primer sprite en la plantilla
	sSprite* spriteQuieto = new sSprite();
	for (int i = 0; i < 3; ++i){
		spriteQuieto->sprites[i].x = 32;
		spriteQuieto->sprites[i].y = 44;
		spriteQuieto->sprites[i].w = 18;
		spriteQuieto->sprites[i].h = 26;
	}
	//this->SpriteMap[QUIETO]=spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO]=spriteQuieto;
    
	// SPRITE CAMINANDO
    x_start = 55; 
	y_start = 44;
	iSpriteInicial = 0;

	sSprite* spriteCaminando = new sSprite();
	//for (int i = 0; i < 3; ++i){
		//spriteCaminando->sprites[i].x = x_start + (i+iSpriteInicial)*iW;
		//spriteCaminando->sprites[i].y = y_start;
		//spriteCaminando->sprites[i].w = iW;
		//spriteCaminando->sprites[i].h = iH;
	//}
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

	//this->SpriteMap[CAMINANDO] = spriteCaminando ;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ] = spriteCaminando ;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER] = spriteCaminando ;

	// SPRITE SALTANDO
	x_start = 55; 
	y_start = 43;
	iSpriteInicial = 5;
	sSprite* spriteSaltando = new sSprite();

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
	//this->SpriteMap[SALTANDO] = spriteSaltando;
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

	for (int i = 0; i < 3; ++i){
		spriteMuriendo->sprites[i].x = x_start + (i+iSpriteInicial)*iW;
		spriteMuriendo->sprites[i].y = y_start;
		spriteMuriendo->sprites[i].w = iW;
		spriteMuriendo->sprites[i].h = iH;
	}
	//this->SpriteMap[MURIENDO]=spriteMuriendo;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_MURIENDO]=spriteMuriendo;

	sSprite* spriteDesconectado = new sSprite();
	
	spriteDesconectado->sprites[0].x = 25;
	spriteDesconectado->sprites[0].y = 486;
	spriteDesconectado->sprites[0].w = 18;
	spriteDesconectado->sprites[0].h = 24;

	spriteDesconectado->sprites[1].x = 25;
	spriteDesconectado->sprites[1].y = 486;
	spriteDesconectado->sprites[1].w = 18;
	spriteDesconectado->sprites[1].h = 24;

	spriteDesconectado->sprites[2].x = 25;
	spriteDesconectado->sprites[2].y = 486;
	spriteDesconectado->sprites[2].w = 18;
	spriteDesconectado->sprites[2].h = 24;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_DESCONECTADO]=spriteDesconectado;
	/*
	//Analizar si este estado va a ser necesario
	//this->SpriteMap[MUERTO]=;

	//this->SpriteMap[LANZANDO_NIEVE]=;
	//this->SpriteMap[EMPUJANDO]=;

*/
}

