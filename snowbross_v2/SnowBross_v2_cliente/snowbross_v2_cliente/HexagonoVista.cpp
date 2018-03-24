#include "StdAfx.h"
#include "HexagonoVista.h"

HexagonoVista::HexagonoVista(sEvento estadoEvento) : PoligonoRegularVista(estadoEvento){
	this->tipo = HEXAGONO;
	this->lados = LADOS_HEXAGONO;

	this->calcularVertices();

	this->rutaTextura = "imagenes/TexturaHexagono.png";

	Logger::getLogger()->debug( "Hexagono Vista creado" );
}