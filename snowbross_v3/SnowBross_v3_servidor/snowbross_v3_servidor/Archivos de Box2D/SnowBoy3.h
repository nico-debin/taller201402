#pragma once

#ifndef _SnowBoy3_H_
#define _SnowBoy3_H_


#include "Personaje3.h"
#include "Nieve3.h"

class Enemigo;

class SnowBoy3: public Personaje{
	private:	
		Enemigo* enemigoBolaTocadaDer;
		Enemigo* enemigoBolaTocadaIzq;

	public:
		SnowBoy3(b2World* mundo,  sObjeto structObj);
		void castearRayosHorizontales();
		Enemigo* analizarBolaTocada(SnowBoyRayCastCallback* callback);
		void disparar();
		
		void atraparEnBolaDeNieve();
		void liberarDeBolaDeNieve(b2Vec2 posicionDeLaBolaDestruida);

		~SnowBoy3(){};

		void actualizar();

};


#endif