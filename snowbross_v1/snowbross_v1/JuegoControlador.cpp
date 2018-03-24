#include "StdAfx.h"
#include "JuegoControlador.h"

#include "EscenarioVista.h"


JuegoControlador::JuegoControlador(void){
	this->parser = nullptr;
	this->escenario = nullptr;
	this->controladorCliente = nullptr;

}

void JuegoControlador::iniciarEscenario(){
	delete this->escenario;
	this->escenario = this->parser->init();
}

void JuegoControlador::iniciar( string nombreArchivoConfiguracion ){
	this->parser = new Parser(nombreArchivoConfiguracion);
	this->iniciarEscenario();
	this->controladorCliente = new ControladorCliente();
	sEventoCliente eventoCliente = {-1,EVENTO_SERVIDOR_NULL};

	this->juegoEnPausa = false;

	while( eventoCliente.evento != EVENTO_SERVIDOR_SALIR ){
		
		if( !this->juegoEnPausa ){
			for( map<int,Figura*>::iterator it = this->escenario->getFiguras()->begin() ; it != this->escenario->getFiguras()->end(); ++it){
				Figura*	figura = (*it).second;
				if( figura->getTipo() == TIPO_SNOWBOY ){
					(( SnowBoy*) figura)->actualizar();
				}
	  		 }
			// Ejecuta un step de box2d					
			escenario->getFisica()->paso();
			
		}

		// Se obtienen actualizaciones de los objetos del escenario
		vector<sEvento*>* vectorEventosServidor = escenario->getEventosEscenario();

		// Se envian a cliente las actualizaciones y se capturan eventos de cliente
		eventoCliente = this->enviarEventosACliente(vectorEventosServidor);

		// Se procesa evento de cliente en el servidor.
		this->procesarEventoCliente( eventoCliente );
        // Sleep recomendado por la catedra para liberar el procesador
        Sleep(1);
	}
}

sEventoCliente JuegoControlador::enviarEventosACliente(vector<sEvento*>* vectorEventosServidor){
	return controladorCliente->procesarEventos(vectorEventosServidor, this->escenario);
}

void JuegoControlador::procesarEventoCliente( sEventoCliente eventoCliente ){
	if( eventoCliente.evento != EVENTO_SERVIDOR_NULL){
		switch(eventoCliente.evento){
        
            case EVENTO_SERVIDOR_SALTAR_PERSONAJE:
                ((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->saltar();
				break;

            case EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE:
                ((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->moverIzquierda();
				break;
            case EVENTO_SERVIDOR_MOVER_DER_PERSONAJE:
                ((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->moverDerecha();
				break;

			case EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE:
                ((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->frenarDerecha();
				break;
			case EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE:
                ((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->frenarIzquierda();
				break;

			case EVENTO_SERVIDOR_PAUSA:
				this->juegoEnPausa = !this->juegoEnPausa;
				vector<sEvento*>* vectorEventos1;
				vectorEventos1 = new vector<sEvento*>;
				sEvento* evento1;
				evento1 = new sEvento;
				//evento->evento = EVENTO_CLIENTE_RESET;
				evento1->evento = EVENTO_CLIENTE_JUEGO_PAUSA;
				vectorEventos1->push_back(evento1);
				this->enviarEventosACliente(vectorEventos1);
				delete evento1;
				delete vectorEventos1;

				break;

            case EVENTO_SERVIDOR_RESET:
				this->iniciarEscenario();
				vector<sEvento*>* vectorEventos2 = new vector<sEvento*>;
				sEvento* evento2 = new sEvento;
				//evento->evento = EVENTO_CLIENTE_RESET;
				evento2->evento = EVENTO_CLIENTE_JUEGO_RESET;
				vectorEventos2->push_back(evento2);
				this->enviarEventosACliente(vectorEventos2);
				delete evento2;
				delete vectorEventos2;

				break;
		}
	}
}

JuegoControlador::~JuegoControlador(void) {
    delete this->parser;
	delete this->escenario;
	delete this->controladorCliente;
}
