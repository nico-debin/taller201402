#pragma once
#include "Enemigo.h"
#include "Utilities.h"


class Diablo:public Enemigo{

	public:
		Diablo(b2World* mundo,  sObjeto structObj);
		virtual Figura* crearProyectil(sObjeto structDelProyectil){return nullptr;};
};
