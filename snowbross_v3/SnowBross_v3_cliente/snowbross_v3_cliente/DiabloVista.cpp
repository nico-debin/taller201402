#include "stdafx.h"
#include "DiabloVista.h"

DiabloVista::DiabloVista(sEvento estadoEvento) : PersonajeVista(estadoEvento){
	this->spriteSheetName = "imagenes/SnowBrosSheet2.gif";
	//HAY QUE SEPARAR ENTRE ENEMIGO,DIABLO Y DRAGON VISTA
	this->tipo = ENEMIGO;
}

void DiabloVista::inicializacionEstados(){
	int iW = 24;
	int iH = 26;

	// Sprites Enemigo Caminando
	sSprite* spriteCaminando = new sSprite();
	spriteCaminando->size = 2;

	spriteCaminando->sprites[0].x = 181;
	spriteCaminando->sprites[0].y = 5;
	spriteCaminando->sprites[0].w = iW;
	spriteCaminando->sprites[0].h = iH;

	spriteCaminando->sprites[1].x = 152;
	spriteCaminando->sprites[1].y = 4;
	spriteCaminando->sprites[1].w = iW;
	spriteCaminando->sprites[1].h = iH;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ] = spriteCaminando;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER] = spriteCaminando;


	// Sprites Enemigo Quieto
	sSprite* spriteQuieto = new sSprite();
	spriteQuieto->size = 1;
	spriteQuieto->sprites[0].x = 122;
	spriteQuieto->sprites[0].y = 4;
	spriteQuieto->sprites[0].w = iW;
	spriteQuieto->sprites[0].h = iH;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO] = spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_CREADO] = spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ] = spriteQuieto;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_QUIETO_DER] = spriteQuieto;


	// Sprites Enemigo Saltando
	sSprite* spriteSaltando = new sSprite();
	spriteSaltando->size = 1;
	iW = 24;
	iH = 31;

	spriteSaltando->sprites[0].x = 151;
	spriteSaltando->sprites[0].y = 33;
	spriteSaltando->sprites[0].w = iW;
	spriteSaltando->sprites[0].h = iH;

	//spriteSaltando->sprites[1].x = 124;
	//spriteSaltando->sprites[1].y = 33;
	//spriteSaltando->sprites[1].w = iW;
	//spriteSaltando->sprites[1].h = iH;

	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO] = spriteSaltando;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ] = spriteSaltando;
	this->SpriteMap[EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER] = spriteSaltando;

	// Sprites Enemigo Muriendo
	sSprite* spriteMuriendo = new sSprite();
	spriteMuriendo->size = 1;
	spriteMuriendo->sprites[0].x = 181;
	spriteMuriendo->sprites[0].y = 33;
	spriteMuriendo->sprites[0].w = iW;
	spriteMuriendo->sprites[0].h = iH;

	this->SpriteMap[EVENTO_CLIENTE_FIGURA_MURIENDO] = spriteMuriendo;

	sSprite* spriteBolaDeNieve = new sSprite();
	spriteBolaDeNieve->size = 1;
	spriteBolaDeNieve->sprites[0].x = 181;
	spriteBolaDeNieve->sprites[0].y = 2094;
	spriteBolaDeNieve->sprites[0].w = 24;
	spriteBolaDeNieve->sprites[0].h = 24;

	this->SpriteMap[EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE] = spriteBolaDeNieve;
	this->SpriteMap[EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA] = spriteBolaDeNieve;

	sSprite* spriteEnemigoConNieve1 = new sSprite();
	spriteEnemigoConNieve1->size = 1;
	spriteEnemigoConNieve1->sprites[0].x = 215;
	spriteEnemigoConNieve1->sprites[0].y = 2172;
	spriteEnemigoConNieve1->sprites[0].w = 24;
	spriteEnemigoConNieve1->sprites[0].h = 24;


	this->SpriteMap[EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1] = spriteEnemigoConNieve1;

	sSprite* spriteEnemigoConNieve2 = new sSprite();
	spriteEnemigoConNieve2->size = 1;
	spriteEnemigoConNieve2->sprites[0].x = 240;
	spriteEnemigoConNieve2->sprites[0].y = 2172;
	spriteEnemigoConNieve2->sprites[0].w = 24;
	spriteEnemigoConNieve2->sprites[0].h = 24;

	this->SpriteMap[EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2] = spriteEnemigoConNieve2;

}

void DiabloVista::dibujar(SDL_Renderer* renderer){
	if (( this->estadoPersonaje == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1 ) || ( this->estadoPersonaje == EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2 )){

		int estadoInicial = this->estadoPersonaje;
		//Le cambio el estado a quieto para dibujarlo
		this->estadoPersonaje = EVENTO_CLIENTE_PERSONAJE_QUIETO;
		PersonajeVista::dibujar(renderer);

		//Le pongo el estado de antes para dibujarle encima la cantidad correcta de nieve
		this->estadoPersonaje = estadoInicial;
		PersonajeVista::dibujar(renderer);

	}
	else{PersonajeVista::dibujar(renderer);}
}
