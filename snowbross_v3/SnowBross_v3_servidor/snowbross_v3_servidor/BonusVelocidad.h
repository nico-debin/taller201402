#pragma once
#include "Bonus.h"

class BonusVelocidad:public Bonus{
private:
	Timer* timerVelocidad;
public:
    BonusVelocidad(b2World* mundo,  sObjeto structObj); 
};