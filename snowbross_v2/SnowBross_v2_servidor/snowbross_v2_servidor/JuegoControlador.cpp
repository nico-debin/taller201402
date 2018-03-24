#include "StdAfx.h"
#include "JuegoControlador.h"

// Thread utilizado para leer los eventos recibidos de los clientes
DWORD WINAPI procesarEventosRecibidos( LPVOID args );

JuegoControlador::JuegoControlador(void){
	this->parser = nullptr;
	this->escenario = nullptr;
}

void JuegoControlador::iniciarEscenario(){
	delete this->escenario;
	this->escenario = this->parser->init();
    this->juegoSalir = false;
    this->juegoEnPausa = false;
}

void JuegoControlador::iniciar( string nombreArchivoConfiguracion, int puerto ){
	this->parser = new Parser(nombreArchivoConfiguracion);
	this->iniciarEscenario();
	this->servidor = new Servidor(puerto);
	this->servidor->start();
	

    // Espero hasta que todos los jugadores se hayan conectado (1,2,3 o 4 segun definción..json?)	
    while( !this->servidor->iniciaServidor ){
		cout << "Sala no esta llena" << endl;
		this->servidor->SalaIncompleta->sleep();
	}

	this->procesarEventoThread = CreateThread( NULL, 0, procesarEventosRecibidos, (LPVOID)this, 0, NULL);
	
	Sleep(500);

	this->procesarEscenario();
}

void JuegoControlador::procesarEscenario(){
    
    while( !this->juegoSalir ){
		
		if( !this->juegoEnPausa ){
			for( map<int,Figura*>::iterator it = this->escenario->getFiguras()->begin() ; it != this->escenario->getFiguras()->end(); ++it){
				Figura*	figura = (*it).second;
				//if( figura->getTipo() == TIPO_SNOWBOY ){
				//	(( SnowBoy*) figura)->actualizar();
				//}
				//Guardo estado anterior para saber si cambio la figura luego del ejecutar el step
				if( figura->cambioEstado() ){
					figura->actualizarEstadoAnterior();
				}
	  		 }
			// Ejecuta un step de box2d


            while(semaforo){Sleep(1);}
			escenario->getFisica()->paso();
            this->semaforo = true;
			
		}

		// Se obtienen actualizaciones de los objetos del escenario
		vector<sEvento*>* vectorEventosServidor = escenario->getEventosEscenario( this->servidor->inicioOReconexion, this->servidor->clientes );

		// De esta forma evitamos reenviar todas las figuras nuevamente, a no ser que se hiciera una reconexion
		this->servidor->inicioOReconexion = false;

		for(std::vector<sEvento*>::iterator it = vectorEventosServidor->begin(); it != vectorEventosServidor->end(); it++){		    
		    // Se envian a cliente las actualizaciones de las figuras
            sEvento* eventoServidor = (*it);
            this->servidor->enqueueEventoEnviar(*eventoServidor);
        }

		sEvento eventoStep;
		memset(&eventoStep,0,sizeof(sEvento));
		eventoStep.id = -1;

		// Envio evento de dibujar a la vista, solo si hay eventos enviados
		if ( vectorEventosServidor->size() > 0 ){
			eventoStep.evento = EVENTO_CLIENTE_JUEGO_STEP;
			this->servidor->enqueueEventoEnviar(eventoStep);
		} else {
			eventoStep.evento = EVENTO_CLIENTE_NULL;
			this->servidor->enqueueEventoEnviar(eventoStep);

		}

        // Sleep recomendado por la catedra para liberar el procesador
        Sleep(1);
	}

}

void JuegoControlador::procesarEventoCliente(){

	Logger::getLogger()->debug("Comenzando thread: procesarEventoCliente()");
	
	sObjeto structPersonaje;
	int divisor;
	Figura* figura;

	if( this->servidor->hayEventosRecibidos() ){

		sEventoCliente eventoCliente = this->servidor->dequeueEventoRecibido();
		sEvento evento;
		ZeroMemory(&evento, sizeof(sEvento));

		std::stringstream str;
		str << "Procesando evento cliente. Id: " << eventoCliente.id << " Evento: " << eventoCliente.evento;
		Logger::getLogger()->debug(str.str());


		if( eventoCliente.evento != EVENTO_SERVIDOR_NULL){
			switch(eventoCliente.evento){
				case EVENTO_SERVIDOR_SALIR:
					for (map<string,Cliente*>::iterator it= this->servidor->clientes.begin(); it!= this->servidor->clientes.end(); ++it){
						if( it->second->idSnowBoy == eventoCliente.id){				
							it->second->estaConectado=false;					
							break;
						}
					}
					break;
				case EVENTO_SERVIDOR_SALTAR_PERSONAJE:
					((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->saltar();
                    //while(!semaforo){Sleep(1);}
                    //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
					break;

				case EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE:
					((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->moverIzquierda();
                    //while(!semaforo){Sleep(1);}
                    //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
					break;

				case EVENTO_SERVIDOR_MOVER_DER_PERSONAJE:
					((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->moverDerecha();
                    //while(!semaforo){Sleep(1);}
                    //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
					break;

				case EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE:
					((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->frenarDerecha();
                    //while(!semaforo){Sleep(1);}
                    //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
					break;

				case EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE:
					((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->frenarIzquierda();
                    //while(!semaforo){Sleep(1);}
                    //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
					break;
	
				//case EVENTO_SERVIDOR_PAUSA:
				//	this->juegoEnPausa = !this->juegoEnPausa;
				//
				//	evento.evento = EVENTO_CLIENTE_JUEGO_PAUSA;
				//	this->servidor->enqueueEventoEnviar(evento);
				//	break;
			}
		}
	}

           //while(!semaforo){Sleep(1);}
        if( semaforo ){
          //((SnowBoy*)(*this->escenario->getFiguras())[eventoCliente.id])->actualizar();
   			for( map<int,Figura*>::iterator it = this->escenario->getFiguras()->begin() ; it != this->escenario->getFiguras()->end(); ++it){
				Figura*	figura = (*it).second;
				if( figura->getTipo() == TIPO_SNOWBOY ){
					(( SnowBoy*) figura)->actualizar();
				}
            }            
            this->semaforo = false;
        }
        

}

JuegoControlador::~JuegoControlador(void) {
    TerminateThread( this->procesarEventoThread, NULL );
    delete this->servidor;
    delete this->parser;
	delete this->escenario;
}

DWORD WINAPI procesarEventosRecibidos( LPVOID p ){
	// Agrega nuevos clientes al servidor
	JuegoControlador* controlador = (JuegoControlador*)p;

	while(true){
		Sleep(7);
        controlador->procesarEventoCliente();
	}
	return 0;
}
