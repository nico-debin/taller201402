#pragma once
#include "Figura.h"
#include "Utilities.h"

class PoligonoRegular : public Figura {
	protected:
		void cargarVertices(b2Vec2 *vertices);
		float calcularArea();

	public:
		PoligonoRegular(b2World* mundo,sObjeto structObj);
		b2Fixture* getFixture();
		void getVertices(float vertices_x[MAX_LADOS], float vertices_y[MAX_LADOS]);
};