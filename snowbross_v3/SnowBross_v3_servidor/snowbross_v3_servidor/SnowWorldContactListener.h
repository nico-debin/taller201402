#ifndef _SnowWorldContactListener_H_
#define _SnowWorldContactListener_H_

#include <Box2D\Box2D.h>
#include "Enemigo.h"
#include "Bonus.h"
#include "SnowBoy.h"
#include <set>

class JuegoControlador;

class SnowWorldContactListener : public b2ContactListener{
	private:
		JuegoControlador* juegoControlador;

		bool compararTipos(TIPO_FIGURA tipoDeFiguraAComparar, TIPO_FIGURA tipoFiguraA,TIPO_FIGURA tipoFiguraB);
		void procesarContactoDeNieve(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoSuperficie(b2Contact* contact, TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoDelSnowBoyConEnemigo(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoDelEnemigoConEnemigo(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoDelEnemigoConEnemigoPreSolve(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoDelEnemigoConPared(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
		void procesarContactoDelSnowBoyConBonus(b2Contact* contact,TIPO_FIGURA tipoFiguraA, TIPO_FIGURA tipoFiguraB, void* objetoA, void* objetoB );
        bool esTipoEnemigo(TIPO_FIGURA tipo);

		set<Figura*> figurasAMatar;
		set<b2Body*> bodiesATeletransportar;

	public:
		SnowWorldContactListener(JuegoControlador* juegoControlador);
		
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

		void matarFiguras();
		void teletransportarBodies();
};


#endif