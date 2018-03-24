#pragma once
#include "Figura.h"
class Rectangulo :public Figura{
	protected:
		float calcularArea();

	public:
		b2Fixture* getFixture();
		Rectangulo(b2World* mundo, float x,float y, float base,float altura, string color,float rotacion, float masa, bool estatico);
		Rectangulo(b2World* mundo,sObjeto structObj);
		Rectangulo();		
		float getBase();
		float getAltura();
		void getVertices(float vertices_x[4], float vertices_y[4]);
};

