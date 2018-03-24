#pragma once
#include "figura.h"
#include <json/json.h>
class Circulo :public Figura{
	protected:
		float calcularArea();

	public:
		b2Fixture* getFixture();
		Circulo(b2World* mundo, float x,float y, float radio, string color,float rotacion, float masa, bool estatico);
		Circulo(b2World* mundo,sObjeto structObj);
		Circulo(float x,float y, float radio, string color,float rotacion, float masa, bool estatico);
		Circulo();
		
		float getRadio();
};

