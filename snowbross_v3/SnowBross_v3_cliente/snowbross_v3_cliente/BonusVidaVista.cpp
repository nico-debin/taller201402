#include "StdAfx.h"
#include "BonusVidaVista.h"


BonusVidaVista::BonusVidaVista(sEvento estadoEvento): Vista(estadoEvento)
{
	this->tipo = VIDA;
	this->lados = LADOS_CUADRADO;

	//this->calcularVertices();

	this->rutaTextura = "imagenes/BonoVida.png";

	Logger::getLogger()->debug( "Bono Vida Vista creado" );
}


BonusVidaVista::~BonusVidaVista(void)
{
}

void BonusVidaVista::setEstado( sEvento estadoEvento ){
	ControlSonido controlSonido;
	//Reproducir sonido de ser levantado
	if ( ( this->estadoActual.evento != EVENTO_CLIENTE_BONUS_DESHABILITADO)  &&  ( estadoEvento.evento == EVENTO_CLIENTE_BONUS_DESHABILITADO) ){
		controlSonido.tocar(ControlRemoto::REPRODUCIR, Temas::BONUS);	
	}

	Vista::setEstado(estadoEvento);
}