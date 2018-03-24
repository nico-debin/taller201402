#ifndef _SaltarRayCastCallback_H_
#define _SaltarRayCastCallback_H_

#include <Box2D\Box2D.h>
#include "estructuraDeDatos.h"

class SaltarRayCastCallback: public b2RayCastCallback{
	private:

		sUserData* userDataDelPersonaje;

	public:
		bool hit;
		b2Vec2 puntoDeContacto,puntoFinalNormal;
		b2Fixture* fixtureTocada;
		b2BodyType tipoDeBodyTocado;
		sUserData* userDataObjetoTocado;

		SaltarRayCastCallback(sUserData* userDataDelPersonaje);

		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		
};


#endif