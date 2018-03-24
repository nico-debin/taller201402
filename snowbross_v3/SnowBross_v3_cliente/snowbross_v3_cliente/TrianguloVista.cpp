#include "stdafx.h"
#include "TrianguloVista.h"

TrianguloVista::TrianguloVista(sEvento estadoEvento) : PoligonoRegularVista(estadoEvento){
	this->tipo = TRIANGULO;
	this->lados = 3;

	this->calcularVertices();

	this->rutaTextura = "imagenes/TexturaTriangulo.png";

	Logger::getLogger()->debug( "Triangulo Vista creado" );
}