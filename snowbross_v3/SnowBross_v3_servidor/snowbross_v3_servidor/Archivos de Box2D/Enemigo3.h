#pragma once

#ifndef _Enemigo3_H_
#define _Enemigo3_H_



#include <Box2D\Box2D.h>
#include "Personaje3.h"
#include <set>

class SnowBoy3;





class Enemigo: public Personaje{
	private:
		int contadorDeIteraciones;

		//pair<TIPO_FIGURA, Enemigo*>* userData;
		sUserData userData;

		int nieveAcumulada;
		int contadorSteps;
		//Debe pasar a ser una cte
		int stepsNecesariosParaPerderNieve;
		//Esta tambien
		int maximaNieveAcumulada;
		//Tambien
		int nieveNecesariaParaFormarBola;
		//ESTO SE PUEDE MANEJAR MEJOR CON ESTADOS
		bool esBola;

		void setearEstado();

		void transformarEnBolaDeNieve();
		void volverAlEstadoNormal();

		set<SnowBoy3*> snowBoysAtrapados;

		//Esto va a ser cte
		int stepsNecesariosParaMorirEnBolaDeNieve;

		void liberarSnowBoysAtrapados();

	public:
		Enemigo(b2World* mundo,  sObjeto structObj);
		~Enemigo();
		void atacar();
		void actualizar();
		void patear(bool patearALaDerecha);
		void atraparSnowBoyEnBolaDeNieve(SnowBoy3* snowBoyAtrapado);
		bool estaMuerto();
		void matarConBolaDeNievePateada();
};


#endif