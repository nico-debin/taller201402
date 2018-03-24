#pragma once
#include "Figura.h"
#include "Utilities.h"


class Paralelogramo:public Figura{
	private:
		void getFormaCuerpo(void);

	protected:
		float calcularArea();
		void calcularVertices(float x, float y);
		void calcularVerticesParalelogramoFisico(float vertices_x[4], float vertices_y[4], float x, float y,float base, float altura, float angulo_inf_izq, float rotacion );

	public:
		Paralelogramo(b2World* mundo,sObjeto structObj);

		b2Fixture* getFixture();
		float getBase();
		float getAltura();
		float getAnguloInclinacion();
		float getRotacionInicial();
		void getVertices(float vertices_x[LADOS_PARALELOGRAMO], float vertices_y[LADOS_PARALELOGRAMO]);
};

