#include "StdAfx.h"
#include "JuegoControlador.h"
#include "Logger.h"

// Thread utilizado para leer los eventos recibidos de los clientes
DWORD WINAPI procesarEventosRecibidos( LPVOID args );

EstadisticasJugadores* JuegoControlador::estadisticasJugadores = new EstadisticasJugadores;

JuegoControlador::JuegoControlador(void){
	this->parser = nullptr;
	this->escenario = nullptr;

	this->juegoFinalizado = false;
	this->juegoFinalizado_cantClientesQueSeDesconectaron = 0;

	// Inidica la cantidad de jugadores que hacen falta dar quorum para iniciar una nueva partida
	this->nuevoJuegoQuorumNecesario = 0;
	this->esperarNuevoJuegoQuorum = new ConditionVariable(new Lock());

	this->contactListener = new SnowWorldContactListener(this);

	this->juegoAux = true;
}

void JuegoControlador::iniciarEscenario(){
	delete this->escenario;
	this->escenario = this->parser->init();
    this->juegoSalir = false;
    this->juegoEnPausa = false;
	this->escenario->getFisica()->getMundo()->SetContactListener(this->contactListener);
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
	
	//Sleep(500);

	this->procesarEscenario();
}

void JuegoControlador::procesarReinicioJuego(){
	// Logica para realizar el reinicio del juego

	const int sizeof_sEvento = sizeof(sEvento);
	this->juegoFinalizado = true;
	this->servidor->setEstadoJuegoFinalizado(true);
	//this->juegoFinalizado_cantClientesQueSeDesconectaron = 0;

	// Elimino los clientes desconectados
	this->juegoFinalizado_cantClientesQueSeDesconectaron =  this->servidor->eliminarClientesDesconectados();

	// Cantiad de jugadores que necesito que den quorum para una nueva partida.
	this->nuevoJuegoQuorumNecesario = Utilities::MAX_JUGADORES - this->juegoFinalizado_cantClientesQueSeDesconectaron;

	// Fix para enviar escenario inicial a los nuevos jugadores que se conecten.
	this->parser->cargarEscenarioInicial(this->escenario);
	
	// Se debe ir cuando tenemos el quorum de los clientes que quedaron conectados
	this->esperarNuevoJuegoQuorum->sleep();

	// Condicion especial cuando hubo desconexiones
	//if(this->juegoFinalizado_cantClientesQueSeDesconectaron > 0){
	if(this->servidor->clientes.size() != Utilities::MAX_JUGADORES ){
		this->servidor->SalaIncompleta->sleep();
	}

	this->estadisticasJugadores->reiniciarEstadisticas();

	/*
	sEvento eventoNivelCompletado;
	memset(&eventoNivelCompletado,0,sizeof_sEvento);
	eventoNivelCompletado.evento = EVENTO_ESCENARIO_NIVEL_COMPLETADO;
	this->servidor->enqueueEventoEnviar(eventoNivelCompletado);
	*/

	this->parser->reiniciarEscenarios();
	//this->cambiarNivel();
	this->cambiarNivelInicial();
	this->juegoFinalizado = false;
	this->servidor->setEstadoJuegoFinalizado(false);
	this->juegoAux = true;

	Sleep(1);
	this->encolarEventosEstadisticas();

	this->nuevoJuegoQuorumNecesario = 0;
	this->juegoFinalizado_cantClientesQueSeDesconectaron = 0;
}

void JuegoControlador::procesarEscenario(){
    
	const int sizeof_sEvento = sizeof(sEvento);
	srand(time(NULL));

    while( !this->juegoSalir && !this->juegoFinalizado ){
		
		if( !this->juegoEnPausa ){
			for( map<int,Figura*>::iterator it = Escenario::getFiguras()->begin() ; it !=  Escenario::getFiguras()->end(); ++it){
				Figura*	figura = (*it).second;

				//Guardo estado anterior para saber si cambio la figura luego del ejecutar el step
				if( figura->cambioEstado() ){
					figura->actualizarEstadoAnterior();
				}
	  		 }
			
			if(this->escenario->getCantidadDeEnemigos() == 0){
				if( this->parser->hayMasEscenarios() ){
					sEvento eventoNivelCompletado;
					memset(&eventoNivelCompletado,0,sizeof_sEvento);
					eventoNivelCompletado.evento = EVENTO_ESCENARIO_NIVEL_COMPLETADO;
					this->servidor->enqueueEventoEnviar(eventoNivelCompletado);

					this->cambiarNivel();
				}else{
					// Si no hay mas escenarios es porque se gano el juego
					sEvento eventoJuegoCompletado;
					memset(&eventoJuegoCompletado,0,sizeof_sEvento);
					eventoJuegoCompletado.evento = EVENTO_ESCENARIO_JUEGO_COMPLETADO;
					this->servidor->enqueueEventoEnviar(eventoJuegoCompletado);

					this->procesarReinicioJuego();
				}
			} else if ( this->juegoFinalizado == false && JuegoControlador::estadisticasJugadores->isGameOver() ){
				sEvento eventoGameOver;
				memset(&eventoGameOver,0,sizeof_sEvento);
				eventoGameOver.evento = EVENTO_ESCENARIO_GAME_OVER;
				this->servidor->enqueueEventoEnviar(eventoGameOver);

				this->procesarReinicioJuego();
			}
			
            while(semaforo){Sleep(1);}

			// Ejecuta un step de box2d
			escenario->getFisica()->paso();
			
			this->contactListener->teletransportarBodies();
			this->contactListener->matarFiguras();

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
		memset(&eventoStep,0,sizeof_sEvento);
		eventoStep.id = -1;

		// Envio evento de dibujar a la vista, solo si hay eventos enviados
		if ( vectorEventosServidor->size() > 0 ){
			eventoStep.evento = EVENTO_CLIENTE_JUEGO_STEP;
			this->servidor->enqueueEventoEnviar(eventoStep);
		} else {
			eventoStep.evento = EVENTO_CLIENTE_NULL;
			//this->servidor->enqueueEventoEnviar(eventoStep);
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

		bool encontrado;
		SnowBoy* snowboy;

		if( eventoCliente.evento != EVENTO_SERVIDOR_NULL){
			switch(eventoCliente.evento){
				case EVENTO_SERVIDOR_SALIR:
					Cliente* cliente;
					encontrado = false;
					for (map<string,Cliente*>::iterator it= this->servidor->clientes.begin(); it!= this->servidor->clientes.end(); ++it){
						if( it->second->idSnowBoy == eventoCliente.id){	
							it->second->estaConectado = false;
							snowboy = (SnowBoy*)Escenario::getFiguras()->at(it->second->idSnowBoy);

							//una para que matar se la reste
							this->estadisticasJugadores->setVidas(eventoCliente.id, 1);
							snowboy->matar();
							//snowboy->inmortal();
							
							this->encolarEventosEstadisticas();
							encontrado = true;
							break;
						}
					}

					if( this->juegoFinalizado && encontrado &&  this->nuevoJuegoQuorumNecesario == 0 ){																		
						this->esperarNuevoJuegoQuorum->wake();
					}
					break;

				case EVENTO_SERVIDOR_RESET:
					if(this->juegoFinalizado){
						this->nuevoJuegoQuorumNecesario--;
						printf("Faltan %d jugadores para iniciar una nueva partida\n", this->nuevoJuegoQuorumNecesario);
						if( this->nuevoJuegoQuorumNecesario == 0 ){
							this->esperarNuevoJuegoQuorum->wake();
						}
					}
					break;

				case EVENTO_SERVIDOR_SALTAR_PERSONAJE:					
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->saltar();}
					break;

				case EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->moverIzquierda();}
					break;

				case EVENTO_SERVIDOR_MOVER_DER_PERSONAJE:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->moverDerecha();}
					break;

				case EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->frenarDerecha();}
					break;

				case EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->frenarIzquierda();}
					break;
				case EVENTO_SERVIDOR_MATAR_FIGURA:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){Escenario::getFiguras()->at(eventoCliente.id)->matar();}
					break;
				case EVENTO_SERVIDOR_SNOWBOY_LANZAR_NIEVE:
					if( Escenario::getFiguras()->find( eventoCliente.id ) != Escenario::getFiguras()->end() ){((SnowBoy*)Escenario::getFiguras()->at(eventoCliente.id))->lanzar();}
					break;
			}
		}
	}

	
	if( this->juegoFinalizado && this->nuevoJuegoQuorumNecesario == 0 ){									
		this->esperarNuevoJuegoQuorum->wake();			
	}
	
    if( semaforo ){
			
				for (map<string,Cliente*>::iterator it= this->servidor->clientes.begin(); it!= this->servidor->clientes.end(); ++it){
						if( it->second->estaConectado == false && !Escenario::getFiguras()->at(it->second->idSnowBoy)->estaMuerta()){	
							this->estadisticasJugadores->setVidas (it->second->idSnowBoy, 0);
							Escenario::getFiguras()->at(it->second->idSnowBoy)->matar();
						}
					}

		

			vector<int> figurasAEliminar;
   			for( map<int,Figura*>::iterator it =  Escenario::getFiguras()->begin() ; it !=  Escenario::getFiguras()->end(); ++it){
				Figura*	figura = (*it).second;
				
				if( figura->getTipo() == TIPO_SNOWBOY ){
					(( SnowBoy*) figura)->actualizar();
				}

				if( figura->getTipo() == TIPO_DIABLO || figura->getTipo() == TIPO_DRAGON ){
					(( Enemigo*) figura)->actualizar();					
				}

				if( figura->getTipo() == TIPO_FUEGO ){
					(( Fuego*) figura)->actualizar();
				}

				if(figura->getTipo() == TIPO_BONUS_VELOCIDAD  ){
					(( BonusVelocidad*) figura)->actualizar();					
				}	

				if(figura->getTipo() == TIPO_BONUS_VIDA ){
					(( BonusVida*) figura)->actualizar();					
				}		


				// Elimino figuras muertas
				if ( figura->estaMuerta()){		
					if(figura->getTipo() == TIPO_BONUS_VELOCIDAD || figura->getTipo() == TIPO_BONUS_VIDA ){
						stringstream stream;
						stream << "Muerta Figura. Id: "<< figura->getId();
					}	
					//TODO: LANZA ERROR AL ELIMINAR FIGURA, ANALIZAR LA UTILIZACION DE LOCK
					//delete Escenario::getFiguras()->at(figura->getId());
					//Escenario::getFiguras()->erase(figura->getId());
					if( figura->getTipo() != TIPO_SNOWBOY ){
						figurasAEliminar.push_back( figura->getId() );
					}
				}
            }
			
			for(std::vector<int>::iterator it = figurasAEliminar.begin(); it != figurasAEliminar.end(); it++){
				Figura* figura = Escenario::getFiguras()->at((*it));
				stringstream stream;
				stream << "Muerta Figura. Id: "<< figura->getId();
				Logger::getLogger()->debug(stream.str());
		
				Escenario::getFiguras()->erase(figura->getId());
				delete figura;
			}
            this->semaforo = false;
        }
        

}

void JuegoControlador::encolarEventosEstadisticas(){
	this->servidor->encolarEventosEstadisticas();
}

void JuegoControlador::cambiarNivel(){
	bool hayNuevoEscenario = this->parser->cargarProximoEscenario(this->escenario);

	// Si el servidor todavia no se puso en pausa, lo espero.
	while(this->servidor->envioDeDatosPausado() == false){
		Sleep(100);
	}

	// Hago un tiempo de espera para que el cliente pueda leer el cartel de cambio de escenario
	Sleep(3000);

	this->servidor->enviarEscenarioClientes();

	Sleep(1);
	this->servidor->quitarPausaEnvioDeDatos();
}

void JuegoControlador::cambiarNivelInicial(){
	//bool hayNuevoEscenario = this->parser->cargarProximoEscenario(this->escenario);
	bool hayNuevoEscenario = this->parser->cargarEscenarioInicial(this->escenario);

	// Si el servidor todavia no se puso en pausa, lo espero.
	while(this->servidor->envioDeDatosPausado() == false){
		Sleep(100);
	}

	// Hago un tiempo de espera para que el cliente pueda leer el cartel de cambio de escenario
	Sleep(3000);

	this->servidor->enviarEscenarioClientes();

	Sleep(1);
	this->servidor->quitarPausaEnvioDeDatos();
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