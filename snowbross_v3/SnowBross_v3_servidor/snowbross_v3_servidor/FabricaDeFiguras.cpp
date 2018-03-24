#include "StdAfx.h"
#include "FabricaDeFiguras.h"


FabricaDeFiguras::FabricaDeFiguras(void)
{
}


FabricaDeFiguras::~FabricaDeFiguras(void)
{
}


Figura* FabricaDeFiguras::crearFigura(b2World* mundo, sObjeto objeto){
	objeto.id = Utilities::getNewID();

	if (objeto.tipo == TIPO_CIRCULO)
		return new Circulo(mundo, objeto);
	else if (objeto.tipo == TIPO_RECTANGULO)
		return new Rectangulo(mundo, objeto);
	else if (objeto.tipo == TIPO_PARALELOGRAMO)
		return new Paralelogramo(mundo, objeto);
	else if (objeto.tipo == TIPO_POLIGONO)
		return new PoligonoRegular(mundo, objeto);
	else if (objeto.tipo == TIPO_TRAPECIO)
		return new Trapecio(mundo, objeto);
    else if (objeto.tipo == TIPO_SNOWBOY)
		return new SnowBoy(mundo, objeto);
	else if (objeto.tipo == TIPO_DIABLO)
		return new Diablo(mundo, objeto);
	else if (objeto.tipo == TIPO_DRAGON)
		return new Dragon(mundo, objeto);	
	else {
		Logger::getLogger()->debug( "Error en la Fabrica de figuras, se intento crear una figura que no estaba definida: " + objeto.tipo);
		return nullptr;
	}
}

Figura* FabricaDeFiguras::crearPersonaje(b2World* mundo, sObjeto personaje){
	personaje.id = Utilities::getNewID();
    return new SnowBoy(mundo, personaje);
}