#include "StdAfx.h"
#include "CuadradoVista.h"

CuadradoVista::CuadradoVista(sEvento estadoEvento) : PoligonoRegularVista(estadoEvento){
	this->tipo = CUADRADO;
	this->lados = LADOS_CUADRADO;

	this->calcularVertices();

	Logger::getLogger()->debug( "Cuadradro Vista creado" );
}