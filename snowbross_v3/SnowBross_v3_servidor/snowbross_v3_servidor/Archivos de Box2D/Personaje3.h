#pragma once
#ifndef _Personaje3_H_
#define _Personaje3_H_


#include "Figura.h"
#include "SnowBoyPiernasCallback.h"
#include "Constantes.h"
#include "estructuraDeDatos.h"


class Personaje: public Figura{
	protected:		

		bool habilitacionIzq;
		bool habilitacionDer;
		bool habilitacionSalto;
	
		bool saltarPresionado;
		bool izqPresionado;
		bool derPresionado;
		bool orientacionDerecha;




		int estado;
		b2Fixture* fixtureDef;
		sObjeto datos;


		void crearForma();

		void castearRayosDePiernas();
		float castearRayo(float posX, float posYInicial, float posYFinal);
		float elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq);

		/*Setea los estados QUIETO_IZQ/DER, SALTANDO_IZQ/DER Y CAMINANDO_IZQ/DER
		  Quieto y Saltando pueden ser comunes al snowBoy y a los enemigos y creo que se setearian de la misma forma
		  de última se puede modificar		
		*/
		virtual void setearEstado(float distanciaAlPiso);


		SnowBoyRayCastCallback* castearRayoHorizontal(float posXFinal);
		virtual void castearRayosHorizontales();
		bool analizarHabilitacionParaRayo(SnowBoyRayCastCallback* callback);

		void desacelerarSiEstaQuieto();

		void analizarMovimientos();

	public:

		Personaje(b2World* mundo,  sObjeto structObj);

		virtual void actualizar();		
		void moverDerecha();
		void moverIzquierda();
		void frenarDerecha();
		void frenarIzquierda();
		void saltar();
		int getEstado();

		b2Fixture* getFixture();
		float getY();
		~Personaje(void);
		float calcularArea();
	//	void disparar();
};

#endif