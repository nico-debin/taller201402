#pragma once
#include "Figura.h"

class Trapecio :public Figura{
	protected:
		float calcularArea();
		void calcularVertices();
		void getFormaCuerpo(void);
		b2Fixture* getFixture();
	public:
		
		Trapecio(b2World* mundo, float x,float y, float baseMayor,float baseMenor, float altura, string color,float rotacion, float masa, bool estatico);
		Trapecio(b2World* mundo,sObjeto structObj);

		float getAnguloIzq();
		float getAnguloDer();
		void getVertices(float vertices_x[LADOS_TRAPECIO], float vertices_y[LADOS_TRAPECIO]);
};
