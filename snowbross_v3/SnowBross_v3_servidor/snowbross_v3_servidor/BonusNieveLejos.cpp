#include "StdAfx.h"
#include "BonusNieveLejos.h"

BonusNieveLejos::BonusNieveLejos(b2World* mundo,  sObjeto structObj):Bonus(mundo,structObj){
    this->datos.tipo=TIPO_BONUS_NIEVE_LEJOS;
    this->userData.tipo=TIPO_BONUS_NIEVE_LEJOS;
}