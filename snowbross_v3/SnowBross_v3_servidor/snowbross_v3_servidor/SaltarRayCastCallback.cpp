#include "StdAfx.h"
#include "SaltarRayCastCallback.h"
#include "Enemigo.h"



SaltarRayCastCallback::SaltarRayCastCallback(sUserData* userDataDelPersonaje){
	hit=false;
	this->userDataObjetoTocado = nullptr;
	this->userDataDelPersonaje = userDataDelPersonaje;
}


float32 SaltarRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	hit = true;
	puntoDeContacto = point;
	puntoFinalNormal=normal;
	tipoDeBodyTocado=fixture->GetBody()->GetType();
	fixtureTocada=fixture;

	if (fixture != nullptr){
		this->userDataObjetoTocado = (sUserData*)fixture->GetBody()->GetUserData();

		if ( this->userDataObjetoTocado != nullptr ){

			TIPO_FIGURA tipoDelObjetoTocado = this->userDataObjetoTocado->tipo;
			TIPO_FIGURA tipoDelPersonaje = this->userDataDelPersonaje->tipo;

			if ( tipoDelObjetoTocado == TIPO_RECTANGULO ){
				((Enemigo*)this->userDataDelPersonaje->objeto)->habilitarSalto();
				return -1;
			} else {
				((Enemigo*)this->userDataDelPersonaje->objeto)->deshabilitarSalto();
				return -1;
			}
			
		}
	}

	return fraction;
}

