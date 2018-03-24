#pragma once
#pragma once

#include <Box2D\Box2D.h>
#include "estructuraDeDatos.h"
#include "Utilities.h"
#include "Constantes.h"
#include "Figura.h"

#include "SnowBoyRayCastCallback.h"

class SnowBoy: public Figura{

	private:
		//b2Body* snowBody;
		bool habilitacionIzq;
		bool habilitacionDer;
		bool habilitacionSalto;

		bool saltarPresionado;
		bool izqPresionado;
		bool derPresionado;
		bool ultimoBotonPresionado; //T si fue el Derecho, F si fue el Izquierdo
		//ESTADO_PERSONAJE estado;
		int estado;

		void crearForma();

		void castearRayosDePiernas();
		void castearRayosHorizontales();
		float castearRayo(float posX, float posYInicial, float posYFinal);
		float elegirDistanciaAlPiso(float distanciaAlPisoDer, float distanciaAlPisoIzq);
		void setearEstado(float distanciaAlPiso);

		bool analizarHabilitacionParaRayo(float posXFinal);

		void analizarMovimientos();
		void desacelerarSiEstaQuieto();

        float calcularArea(){ return 4;};
        b2Fixture* getFixture();

	public:

		SnowBoy(b2World* mundo, sObjeto structObj);
		void actualizar();
		void moverDerecha();
		void moverIzquierda();


		void frenarDerecha();
		void frenarIzquierda();



		void saltar();
		//ESTADO_PERSONAJE getEstado();
		int getEstado();
		
		float32 getY();

		~SnowBoy(void);
};




