#pragma once
#include "Vista.h"
#include "ControlSonido.h"

class BonusVidaVista: public Vista{
public:
	BonusVidaVista(sEvento estadoEvento);
	~BonusVidaVista(void);

	virtual void setEstado( sEvento estadoEvento );
};
