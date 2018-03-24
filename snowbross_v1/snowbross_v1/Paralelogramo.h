#pragma once
#include "Figura.h"
#include "Utilities.h"


class Paralelogramo:public Figura{
	private:
		void getFormaCuerpo(void);
	protected:
		float calcularArea();
		void calcularVertices(float x, float y);
	public:
		b2Fixture* getFixture();
		Paralelogramo(b2World* mundo, float x,float y, float base,float altura, float anguloInclinacion, string color,float rotacion, float masa, bool estatico);
		Paralelogramo(b2World* mundo,sObjeto structObj);
		float getBase();
		float getAltura();
		float getAnguloInclinacion();
		float getRotacionInicial();
		void getVertices(float vertices_x[LADOS_PARALELOGRAMO], float vertices_y[LADOS_PARALELOGRAMO]);
};

