#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
#include <math.h>
#include "Fisica.h"
#include "estructuraDeDatos.h"

#include "Utilities.h"

using namespace std;

class Figura: public b2ContactListener{
	
	protected:
		sObjeto datos;

		//int id;
		//string tipo;
		/*
		string color;
		float ancho;
		float alto;
		float rotacion;
		float masa;
		float escala;
		bool estatico;
		*/

		float gradosARadianes(float grados);
		float calcularDensidad();
		virtual float calcularArea()=0;
		
		
	public:
		b2Body* body;
		b2Fixture* fixtureDef;

		float verticesX[MAX_LADOS];
		float verticesY[MAX_LADOS];

		Figura();
		Figura(b2World* mundo, float x,float y, string color,float rotacion, bool estatico);
		Figura(b2World* mundo,sObjeto structObj);

		int getId();

		sObjeto getDatos();

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
		~Figura(void);
		void dibujar(SDL_Renderer *renderer);
};

