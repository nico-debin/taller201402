#pragma once
#ifndef _DisparoContactListener_H_
#define _DisparoContactListener_H_


#include <Box2D\Box2D.h>
#include "Enemigo3.h"
#include <set>


class DisparoContactListener : public b2ContactListener{
	
	private:
		bool esTipoEnemigo(TIPO_FIGURA tipo);

		//ESTO PUEDE SER DE JUEGO CONTROLADOR
		set<b2Body*> bodiesDeNieveAEliminar;
		//AHORA ESTO SE LIMPIA EN EL ACTUALIZAR DE LA TEST
		//O que en el actualizar se le pida esta lista para limpiarla
		

		/*Analiza el contacto de la Nieve con cualquier cosa*/
		void procesarContactoDeNieve(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		/*Analiza el contacto del SnowBoy con una bola pateada, falta SB-Enemigo*/
		void procesarContactoDelSnowBoy(TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		/*Analiza el contacto de un Enemigo con una bola pateada*/
		void procesarContactoDelEnemigo(TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		bool compararTipos(TIPO_FIGURA tipoDeFiguraAComparar, TIPO_FIGURA tipoFiguraA,TIPO_FIGURA tipoFiguraB);
	public:
		

	    void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

		void eliminarBodies();
};

#endif