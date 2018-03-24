#pragma once
#include "Figura.h"
#include "Utilities.h"


class Proyectil:public Figura{
	protected:
		float calcularArea(){ return 1;}
		
		void modificarBody();

		virtual void crearForma()=0;
        virtual void setearVelocidad()=0;

		void setearEstado();

		bool tieneOrientacionDerecha();

		bool habilitacionParaGolpear;

	public:
		Proyectil(b2World* mundo,  sObjeto structObj);
		//Por qué es abstracta?
		b2Fixture* getFixture();


		//Esto es para que no pueda atacar algo dos veces
		bool puedeGolpear();
		void deshabilitar();

};
