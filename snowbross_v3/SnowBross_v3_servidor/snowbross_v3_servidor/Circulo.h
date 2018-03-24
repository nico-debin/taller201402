#pragma once
#include "figura.h"
#include <json/json.h>
class Circulo :public Figura{
	protected:
		float calcularArea();

	public:
		Circulo(b2World* mundo,sObjeto structObj);

		b2Fixture* getFixture();
		float getRadio();
};

