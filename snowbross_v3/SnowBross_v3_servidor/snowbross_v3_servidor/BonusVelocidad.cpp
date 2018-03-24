#include "StdAfx.h"
#include "BonusVelocidad.h"

BonusVelocidad::BonusVelocidad(b2World* mundo,  sObjeto structObj):Bonus(mundo,structObj){
    this->datos.tipo=TIPO_BONUS_VELOCIDAD;
    this->userData.tipo = TIPO_BONUS_VELOCIDAD;
	//this->estado = TIPO_BONUS_HABILITADO;
}
