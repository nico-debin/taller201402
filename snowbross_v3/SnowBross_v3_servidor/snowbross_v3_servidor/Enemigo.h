#pragma once
#include "Personaje.h"
#include "Utilities.h"
#include "stdlib.h"
#include "time.h"
#include <set>

class SnowBoy;

class Enemigo:public Personaje{

	private:
		int contadorDeIteraciones;
		int nieveAcumulada;
		int contadorSteps;
		//Debe pasar a ser una cte
		int stepsNecesariosParaPerderNieve;
		//Esta tambien
		int maximaNieveAcumulada;
		//Tambien
		int limiteDeNieve1;
		int limiteDeNieve2;
		int nieveNecesariaParaFormarBola;
		//ESTO SE PUEDE MANEJAR MEJOR CON ESTADOS
		bool esBola;

		Timer timerSalto;

		void setearEstado();//No se si es necesario

		void transformarEnBolaDeNieve();
		void volverALaFormaNormal();

		//Esto va a ser cte
		int stepsNecesariosParaMorirEnBolaDeNieve;

		set<SnowBoy*> snowBoysAtrapados;
		void liberarSnowBoysAtrapados();
		


		void setearEstado(float distanciaAlPiso);

		//void cargarBitsDeColisionEnFixtures(uint16 categoryBits, uint16 maskBits);
		void cargarBitsDeColisionDeEnemigoNormal();
		void cargarBitsDeColisionDeEnemigoConNieve();
		void cargarBitsDeColisionDeEnemigoBolaDeNieveQuieta();
		void cargarBitsDeColisionDeEnemigoBolaDeNievePateada();

		void crearBono();
		sObjeto inicializoFigura();

	public:
		Enemigo (b2World* mundo,  sObjeto structObj);
		void serAtacadoConNieve();
		void actualizar();
		bool puedeMatar();

		void patear(bool patearALaDerecha);
        void crearBonus();

		void atraparSnowBoyEnBolaDeNieve(SnowBoy* snowBoyAtrapado);
		void liberarSnowBoy(SnowBoy* snowBoyAtrapado);

		~Enemigo();
		

};