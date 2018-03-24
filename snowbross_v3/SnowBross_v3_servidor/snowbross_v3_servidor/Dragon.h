#pragma once
#include "Enemigo.h"
#include "Utilities.h"


class Dragon:public Enemigo{
private:
		Timer* timerFuego;
		Timer* timerQuieto;
public:
    Dragon(b2World* mundo,  sObjeto structObj);
	~Dragon();
	virtual Figura* crearProyectil(sObjeto structDelProyectil);
	void analizarLanzarFuego();
};
