#pragma once
#include "Figura.h"
#include "Utilities.h"

class Bonus:public Figura{
private:
	int tiempoEsperaSinLevantar;

	bool habilitacion;

public:
	Bonus(b2World* mundo,  sObjeto structObj);
	float calcularArea(){ return 4;}
	b2Fixture* getFixture(){ return this->fixtureDef;} 
	void actualizar();

	void deshabilitar();
	bool estaHabilitado();
};
