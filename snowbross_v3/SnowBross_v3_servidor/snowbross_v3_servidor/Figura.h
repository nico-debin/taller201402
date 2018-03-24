#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <math.h>
#include "Fisica.h"
#include "estructuraDeDatos.h"

#include "Utilities.h"
#include "Timer.h"
#include "Constantes.h"

using namespace std;

class Figura: public b2ContactListener{
	
	protected:
		sObjeto datos;
		int estado;

		sObjeto datosEstadoAnterior;
		int estadoAnterior;
		
		Timer* timerAgonia;
		
		bool estaMuerto;

		float gradosARadianes(float grados);
		float calcularDensidad();
		virtual float calcularArea()=0;
		void setEstadoAnterior( sObjeto datosEstadoAnterior);
		
		sUserData userData;

	public:
		b2Body* body;
		b2Fixture* fixtureDef;
		int tiempoAgonia;

		float verticesX[MAX_LADOS];
		float verticesY[MAX_LADOS];

		Figura(b2World* mundo,sObjeto structObj);
		~Figura(void);

		int getId();

		virtual sObjeto getDatos();

		TIPO_FIGURA getTipo();
		float32 getX();
		virtual float32 getY();
		float getAncho();
		float getAlto();
		// Return: angulo en radianes
		float32 getAngulo();
		bool esEstatico();
		void setFiguraCuerpo(b2Body* figuraCuerpo);
		b2Body* getFiguraCuerpo();
		virtual b2Fixture* getFixture()=0;
		bool cambioEstado();
		virtual void actualizarEstadoAnterior(); 
		void actualizarEstado();
		virtual void setEstado( int estado );
		virtual int getEstado();

		virtual void matar();
		void muerto();
		virtual bool estaMuerta();
		bool estaViva();
};

