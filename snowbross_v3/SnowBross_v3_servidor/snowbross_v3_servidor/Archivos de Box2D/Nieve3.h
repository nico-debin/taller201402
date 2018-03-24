#pragma once
#ifndef _Nieve_H_
#define _Nieve_H_

#include <Box2D\Box2D.h>
#include "Constantes.h"
#include "estructuraDeDatos.h"
#include "Figura.h"

class Nieve : public Figura{
	/*ctes:
		NieveOffsetX
		NieveOffsetY
		NieveRadio
		NieveVelocidadHorizontal
	*/
	private:
		
		Timer tiempoAgonia;

		b2Body* body;
		void crearBody(b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha);
		void crearForma();
		void setearVelocidad(bool snowBoyOrientacionDerecha);
	protected:
		float calcularArea(){return 1;}
	public:							
		Nieve( b2Body* bodyDelSnowBoy, bool snowBoyOrientacionDerecha);
		~Nieve();

		static set<Nieve*> vectorDeNieves;
		static void limpiarNieves();
		
		
		static void getNievesVivas(){ //En get Eventos escenario,, return set de nievesVivas
			//set nievesVivas
			//recorremos el set de nieves
			//if nieveActual->estaMuerta-->eliminarDelVector
			//else{ nievesVivas.insert(nieveActual)
			//return nievesVivas
		}
		static void agregarNuevaNieve(Nieve* nieveNueva);//En el disparar
};

#endif