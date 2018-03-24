#include "StdAfx.h"
#include "BonusVida.h"

BonusVida::BonusVida(b2World* mundo,  sObjeto structObj):Bonus(mundo,structObj){
    this->datos.tipo=TIPO_BONUS_VIDA;
    this->userData.tipo = TIPO_BONUS_VIDA;
	//this->estado = TIPO_BONUS_HABILITADO;
}