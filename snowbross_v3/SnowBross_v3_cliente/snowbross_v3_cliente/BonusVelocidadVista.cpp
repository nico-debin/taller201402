#include "StdAfx.h"
#include "BonusVelocidadVista.h"


BonusVelocidadVista::BonusVelocidadVista(sEvento estadoEvento): Vista(estadoEvento)
{
	this->tipo = VELOCIDAD;
	this->lados = LADOS_CUADRADO;

	//this->calcularVertices();

	this->rutaTextura = "imagenes/BonoVelocidad.png";

	Logger::getLogger()->debug( "Bono Vida Vista creado" );
}


BonusVelocidadVista::~BonusVelocidadVista(void)
{
}

void BonusVelocidadVista::setEstado( sEvento estadoEvento ){
	ControlSonido controlSonido;
	//Reproducir sonido de ser levantado
	if ( ( this->estadoActual.evento != EVENTO_CLIENTE_BONUS_DESHABILITADO)  &&  ( estadoEvento.evento == EVENTO_CLIENTE_BONUS_DESHABILITADO) ){
		controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::BONUS);	
	}

	Vista::setEstado(estadoEvento);
}
