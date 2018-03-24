#pragma once
#ifndef _Figure_H_
#define _Figura_H_

#include <Box2D/Box2D.h>
#include <iostream>
#include <math.h>
#include "Fisica.h"
#include "estructuraDeDatos.h"


using namespace std;

class Figura : public Fisica {
	
	protected:
		b2Body* body;
		string color;	
		float rotacion;
		float escala;
		bool estatico;
		float verticesX[5];
		float verticesY[5];

		float gradosARadianes(float grados);
		float calcularDensidad(float masa);
		virtual float calcularArea()=0;
		//virtual void calcularVertices(void)=0;
		
	public:
		Figura();
		Figura(b2World* mundo, float x,float y, string color,float rotacion, bool estatico);
		Figura(b2World* mundo,sObjeto structObj);
		float32 getX();
		virtual float32 getY();
		// Return: angulo en radianes
		float32 getAngulo();
		bool esEstatico();
		void setFiguraCuerpo(b2Body* figuraCuerpo);
		b2Body* getBody();
		~Figura(void);
};

#endif