#include "StdAfx.h"
#include "SnowBoyRayCastCallback.h"

SnowBoyRayCastCallback::SnowBoyRayCastCallback(void){
	hit=false;
}


float32 SnowBoyRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	hit = true;
	puntoDeContacto = point;
	puntoFinalNormal=normal;
	tipoDeBodyTocado=fixture->GetBody()->GetType();
	fixtureTocada=fixture;

	if (fixture->GetFilterData().groupIndex == -1){
		fixtureTocada = false;
		hit = false;
	}

	return fraction;
}


SnowBoyRayCastCallback::~SnowBoyRayCastCallback(void)
{
}
