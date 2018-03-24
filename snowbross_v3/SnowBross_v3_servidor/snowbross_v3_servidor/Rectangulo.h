#pragma once
#include "Figura.h"
class Rectangulo :public Figura{
	protected:
		float calcularArea();

	public:
		Rectangulo(b2World* mundo,sObjeto structObj);

		b2Fixture* getFixture();
		float getBase();
		float getAltura();
		void getVertices(float vertices_x[4], float vertices_y[4]);
};

