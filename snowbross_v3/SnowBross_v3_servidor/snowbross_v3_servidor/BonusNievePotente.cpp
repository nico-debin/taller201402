#include "StdAfx.h"
#include "BonusNievePotente.h"

BonusNievePotente::BonusNievePotente(b2World* mundo,  sObjeto structObj):Bonus(mundo,structObj){
    this->datos.tipo=TIPO_BONUS_NIEVE_POTENTE;
    this->userData.tipo=TIPO_BONUS_NIEVE_POTENTE;
}