#ifndef _SnowBoyRayCastCallback_H_
#define _SnowBoyRayCastCallback_H_

#include <Box2D\Box2D.h>
#include "estructuraDeDatos.h"

class SnowBoyRayCastCallback: public b2RayCastCallback{
	private:
		bool esTipoEnemigo(TIPO_FIGURA tipo);
		void anularContactoDelRaycast();
		bool enemigoEsBolaDeNieve();

		sUserData* userDataDelPersonaje;
	public:
		bool hit;
		b2Vec2 puntoDeContacto,puntoFinalNormal;
		b2Fixture* fixtureTocada;
		b2BodyType tipoDeBodyTocado;
		sUserData* userDataObjetoTocado;

		//SnowBoyRayCastCallback(void);
		SnowBoyRayCastCallback(sUserData* userDataDelPersonaje);

		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		~SnowBoyRayCastCallback(void);
		
};


#endif