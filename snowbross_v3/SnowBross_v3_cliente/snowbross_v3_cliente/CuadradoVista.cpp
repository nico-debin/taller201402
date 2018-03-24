#include "StdAfx.h"
#include "CuadradoVista.h"

CuadradoVista::CuadradoVista(sEvento estadoEvento) : PoligonoRegularVista(estadoEvento){
	this->tipo = CUADRADO;
	this->lados = LADOS_CUADRADO;

	this->calcularVertices();
	
	this->rutaTextura = "imagenes/TexturaCuadrado.png";

	Logger::getLogger()->debug( "Cuadradro Vista creado" );
}