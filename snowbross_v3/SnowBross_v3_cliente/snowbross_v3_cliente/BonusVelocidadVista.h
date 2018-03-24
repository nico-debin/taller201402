#pragma once
#include "Vista.h"
#include "ControlSonido.h"

class BonusVelocidadVista: public Vista
{
public:
	BonusVelocidadVista(sEvento estadoEvento);
	~BonusVelocidadVista(void);

	virtual void setEstado( sEvento estadoEvento );
};

