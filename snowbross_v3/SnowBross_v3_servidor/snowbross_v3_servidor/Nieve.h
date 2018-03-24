#pragma once
#include "Proyectil.h"
#include "Utilities.h"


class Nieve:public Proyectil{
	protected:
		//float calcularArea(){ return 1;}
		//
		//void modificarBody();
		virtual void crearForma();
		virtual void setearVelocidad();
		//void setearEstado();

		//bool tieneOrientacionDerecha();

		//bool habilitacionParaGolpear;

	public:
		//Nieve(b2World* mundo,  sObjeto structObj);
        Nieve(b2World* mundo,  sObjeto structObj);
		
		//Esto se usa para que la nieve vaya un poco mas rapido si el snowboy agarro el bonus de velocidad
		void aplicarInercia();

		//Por qué es abstracta?
		//b2Fixture* getFixture();


		////Esto es para que no pueda atacar algo dos veces
		//bool puedeGolpear();
		//void deshabilitar();

};
