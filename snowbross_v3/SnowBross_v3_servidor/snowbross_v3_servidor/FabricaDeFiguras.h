#pragma once

#include "StdAfx.h"
#include "Figura.h"
#include "Rectangulo.h"
#include "Circulo.h"
#include "Paralelogramo.h"
#include "Trapecio.h"
#include "PoligonoRegular.h"
#include "SnowBoy.h"
#include "Constantes.h"
#include <json/json.h>
#include "Logger.h"
#include "Diablo.h"
#include "Dragon.h"
#include "BonusVida.h"
#include "BonusVelocidad.h"


class FabricaDeFiguras
{
public:
	
	FabricaDeFiguras(void);
	~FabricaDeFiguras(void);
	static Figura* crearFigura(b2World* mundo, sObjeto objeto);
    static Figura* crearPersonaje(b2World* mundo, sObjeto personaje);
	
};


