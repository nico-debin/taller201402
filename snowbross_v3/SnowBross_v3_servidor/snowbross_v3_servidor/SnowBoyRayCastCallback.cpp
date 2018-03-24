#include "StdAfx.h"
#include "SnowBoyRayCastCallback.h"
#include "Enemigo.h"
#include "Nieve.h"
#include "BonusVida.h"
#include "BonusVelocidad.h"
#include "SnowBoy.h"

/*SnowBoyRayCastCallback::SnowBoyRayCastCallback(void){
	hit=false;
	this->userDataObjetoTocado = nullptr;
}*/

SnowBoyRayCastCallback::SnowBoyRayCastCallback(sUserData* userDataDelPersonaje){
	hit=false;
	this->userDataObjetoTocado = nullptr;
	this->userDataDelPersonaje = userDataDelPersonaje;
}


float32 SnowBoyRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
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

			if (tipoDelObjetoTocado == TIPO_BONUS_VIDA){
				/*if (tipoDelPersonaje == TIPO_SNOWBOY){
					BonusVida* bonusVidaLevantado = (BonusVida*)this->userDataObjetoTocado->objeto;
					if (bonusVidaLevantado->estaHabilitado()){					
						SnowBoy* snowBoy = (SnowBoy*)this->userDataDelPersonaje->objeto;
						snowBoy->incrementarVida();
						bonusVidaLevantado->deshabilitar();
						bonusVidaLevantado->matar();
					}
				
				}*/
				this->anularContactoDelRaycast();
				return -1;
			}
			else if (tipoDelObjetoTocado == TIPO_BONUS_VELOCIDAD ){
				/*if (tipoDelPersonaje == TIPO_SNOWBOY){
					BonusVelocidad* bonusVelocidadLevantado = (BonusVelocidad*)this->userDataObjetoTocado->objeto;
					if (bonusVelocidadLevantado->estaHabilitado()){					
						SnowBoy* snowBoy = (SnowBoy*)this->userDataDelPersonaje->objeto;
						snowBoy->incrementarVelocidad();
						bonusVelocidadLevantado->deshabilitar();
						bonusVelocidadLevantado->matar();
					}	
				
				}*/
				this->anularContactoDelRaycast();
				return -1;
			}

			else if (tipoDelObjetoTocado == TIPO_SNOWBOY || tipoDelObjetoTocado == TIPO_PISO || tipoDelObjetoTocado == TIPO_FUEGO){

				this->anularContactoDelRaycast();
				return -1;
			}
			else if (tipoDelObjetoTocado == TIPO_NIEVE){

				Nieve* nieveTocada = (Nieve*)this->userDataObjetoTocado->objeto;

				if ( this->esTipoEnemigo(tipoDelPersonaje) && nieveTocada->puedeGolpear()){
					((Enemigo*) this->userDataDelPersonaje->objeto)->serAtacadoConNieve();
					nieveTocada->deshabilitar();
				}
				this->anularContactoDelRaycast();
				return -1;
			}
			else if (this->esTipoEnemigo(tipoDelObjetoTocado)){

				if ( ! (tipoDelPersonaje == TIPO_SNOWBOY && this->enemigoEsBolaDeNieve()) ){
					this->anularContactoDelRaycast();				
					return -1;					
				}			
			
			}
		}
		else{ //UserData == nullptr
			this->anularContactoDelRaycast();				
			return -1;
		}
	}



	/*if (fixture->GetFilterData().groupIndex == -1){
		fixtureTocada = false;
		hit = false;
	}*/

	

	return fraction;
}


SnowBoyRayCastCallback::~SnowBoyRayCastCallback(void)
{
}

bool SnowBoyRayCastCallback::esTipoEnemigo(TIPO_FIGURA tipo){
	return ( tipo == TIPO_DIABLO || tipo == TIPO_DRAGON );
}

void SnowBoyRayCastCallback::anularContactoDelRaycast(){
	fixtureTocada = false;
	hit = false;
}

bool SnowBoyRayCastCallback::enemigoEsBolaDeNieve(){
	
	int estadoDelEnemigo = ( (Enemigo*) this->userDataObjetoTocado->objeto)->getEstado();

	return (estadoDelEnemigo == EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE);
}