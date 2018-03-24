#pragma once
#include <Box2D\Box2D.h>

class SnowBoyRayCastCallback: public b2RayCastCallback{
	public:
		bool hit;
		b2Vec2 puntoDeContacto,puntoFinalNormal;
		b2Fixture* fixtureTocada;
		b2BodyType tipoDeBodyTocado;

		SnowBoyRayCastCallback(void);
		float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
		~SnowBoyRayCastCallback(void);
		
};
