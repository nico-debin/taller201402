#pragma once
#include "Proyectil.h"
#include "Utilities.h"


class Fuego:public Proyectil{
protected:
    virtual void crearForma();
    virtual void setearVelocidad();	

public:
	int tiempoFuego;
    Fuego(b2World* mundo,  sObjeto structObj);
	void actualizar();

};
