#include "SnowBoyPiernasCallback.h"


SnowBoyRayCastCallback::SnowBoyRayCastCallback(void){
	hit=false;
	this->userDataObjetoTocado = nullptr;
}


float32 SnowBoyRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	hit = true;
	puntoDeContacto = point;
	puntoFinalNormal=normal;
	tipoDeBodyTocado=fixture->GetBody()->GetType();
	
	fixtureTocada=fixture;

	
	if (fixture != nullptr){
		this->userDataObjetoTocado = (sUserData*)fixture->GetBody()->GetUserData();
		/*if (this->userDataObjetoTocado->tipo == TIPO_ENEMIGO_BOLA_DE_NIEVE){
			printf("TOCA A UNA BOLA DE NIEVE         \n");
		}*/	
	}

	if (fixture->GetFilterData().groupIndex == -1){
		fixtureTocada = false;
		hit = false;
	}


	return fraction;
}


SnowBoyRayCastCallback::~SnowBoyRayCastCallback(void)
{
}
