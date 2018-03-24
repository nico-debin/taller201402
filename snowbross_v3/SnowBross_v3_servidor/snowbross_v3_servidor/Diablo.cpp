#include "StdAfx.h"
#include "Diablo.h"

Diablo::Diablo(b2World* mundo,  sObjeto structObj): Enemigo(mundo, structObj){
	this->userData.tipo = TIPO_DIABLO;
}