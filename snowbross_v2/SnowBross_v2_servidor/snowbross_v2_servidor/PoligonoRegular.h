#pragma once
#include "Figura.h"
#include "Utilities.h"

class PoligonoRegular : public Figura {
	protected:
		void cargarVertices(b2Vec2 *vertices);
		float calcularArea();
	public:
		b2Fixture* getFixture();
		PoligonoRegular(b2World* mundo, float x,float y, int lados,float escala, string color,float rotacion, float masa, bool estatico);
		PoligonoRegular(b2World* mundo,sObjeto structObj);
		void getVertices(float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS]);
};