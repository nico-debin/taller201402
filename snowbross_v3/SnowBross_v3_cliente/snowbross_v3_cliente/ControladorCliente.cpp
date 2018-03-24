#include "stdafx.h"
#include "ControladorCliente.h"
#include "EscenarioVista.h"
#include "CirculoVista.h"
#include "RectanguloVista.h"
#include "ParalelogramoVista.h"
#include "TrianguloVista.h"
#include "CuadradoVista.h"
#include "PentagonoVista.h"
#include "HexagonoVista.h"
#include "TrapecioVista.h"
#include "SnowBoyVista.h"
#include "DiabloVista.h"
#include "DragonVista.h"
#include "NieveVista.h"
#include "FuegoVista.h"
#include "BonusVelocidadVista.h"
#include "BonusVidaVista.h"
#include "Timer.h"

// Thread utilizado para leer los eventos enviados por los clientes
DWORD WINAPI capturarEventos( LPVOID args );
DWORD WINAPI actualizarPantalla( LPVOID args );

EstadisticasJugadores* ControladorCliente::estadisticasJugadores = new EstadisticasJugadores;

ControladorCliente::ControladorCliente( string username, string ip, int puerto ){
	Utilities::ANCHO_PX = 900;
	Utilities::ALTO_PX = 300;

	Utilities::ANCHO_UL = 100;
	Utilities::ALTO_UL = 50;

	Utilities::PAUSA_CLIENTE = false;
	this->juegoSalir = false;
	this->KeyPress_left = false;
	this->KeyPress_right = false;
	this->KeyPress_up = false;
	this->KeyPress_space = false;

	this->escenarioVista = nullptr;
    
    this->cliente = new Cliente( username, ip, puerto );

	
	this->imprimiendoPantalla = false;
	this->imprimiendoPantallaCV = new ConditionVariable();
	this->actualizandoObjetoCV = new ConditionVariable(this->imprimiendoPantallaCV->getLock());
	this->actualizandoObjeto = false;
	this->firstStep = false;

	this->esperandoNuevoEscenario = false;
	this->juegoFinalizado = false;
	this->resetEnviado = false;
}

ControladorCliente::~ControladorCliente(){
    TerminateThread( this->capturarEventoThread, NULL );
	TerminateThread( this->actualizarPantallaThread, NULL );
	delete this->cliente;
	//delete this->imprimiendoPantallaCV;
	//delete this->actualizandoObjetoCV;
	if( this->escenarioVista != nullptr ){
		delete this->escenarioVista;
	}

}

void ControladorCliente::iniciar(){
    if( this->cliente->conectarseConServidor() ) {
		if( cliente->loginServidor() ){ 
			cliente->iniciar();

			// El login captura los datos del escenario, por eso se instancia aca EscenarioVista.
			//Logger::getLogger()->info("Esperando que se terminen de unir todos los jugadores...");
			//this->cliente->hayEventosRecibidos->sleep();

			Logger::getLogger()->info("Iniciando juego.");
			this->escenarioVista = new EscenarioVista();

			this->actualizarPantallaThread = CreateThread( NULL, 0, actualizarPantalla, (LPVOID)this, 0, NULL);
			this->capturarEventoThread = CreateThread( NULL, 0, capturarEventos, (LPVOID)this, 0, NULL);

			//this->procesarEventos();
			this->obtenerEventoPantalla();
		}else{
			system("pause");
		}
	} else {
		stringstream info;
		info << "No se pudo conectar con el servidor luego de [" << cliente->MAX_INTENTOS_CONEXION << "] intentos.\n";
		Logger::getLogger()->error( info.str() );
	}
}

ControladorCliente::ControladorCliente( EscenarioVista* escenarioVista){
    this->setEscenarioVista(escenarioVista);
	//this->juegoEnPausa = false;
	Utilities::PAUSA_CLIENTE = false;
}

void ControladorCliente::setEscenarioVista(EscenarioVista* escenarioVista){
	this->escenarioVista = escenarioVista;
}


void ControladorCliente::procesarEventos(){

    while( !this->juegoSalir ){
		if(this->cliente->getCantidadEventosRecibidos() == 0){
			this->cliente->hayEventosRecibidos->sleep();
		}

        sEvento eventoServidor = this->cliente->dequeueEventoRecibido();

		if( (eventoServidor.evento < -1) || (eventoServidor.evento > 300) ){
			cout << "Continue...." << endl;
			continue;
		}

        if (eventoServidor.id != -1 ){
			stringstream stream;
			stream << "Msg Recibido. Id: "<< eventoServidor.id <<" Evento: "<< eventoServidor.evento << " tipo: " << eventoServidor.tipo;
			Logger::getLogger()->debug(stream.str());
        }
		
		if( this->esperandoNuevoEscenario && this->cliente->esperarNuevoEscenario == false ){
			// Se recibio un nuevo escenario. Debo eliminar las vistas anteriores.
			this->escenarioVista->eliminarObjetos(false);

			// Dejo de mostrar el cartel de cambio de escenario por sdl.
			this->esperandoNuevoEscenario = false;
			this->escenarioVista->statusMostrarEscenarioCompletado = false;

			//**********

				this->escenarioVista->statusMostrarEscenarioGameOver = false;
				this->escenarioVista->statusMostrarJuegoCompletado = false;
				this->escenarioVista->statusMostrarEscenarioCompletado = false;
				// Indica al controlador que el juego finalizo, y espera un posible reinicio o exit
				this->juegoFinalizado = false;
				this->resetEnviado = false;

				// Indica al socket que debe esperar por struct especial de escenario
				this->cliente->esperarNuevoEscenario = false;

				// Flag para saber cuando dejar de mostrar el cartel por sdl
				this->esperandoNuevoEscenario = false;

				this->escenarioVista->statusMostrarCartelTeclas = false;

			
		}
		

		switch(eventoServidor.evento){
			case EVENTO_CLIENTE_NULL:
				break;
			case EVENTO_CLIENTE_JUEGO_SALIR:
				this->juegoSalir = true;
				SDL_Event evento;
				evento.type = SDL_QUIT;
				SDL_PushEvent(&evento);
				break;
			case EVENTO_CLIENTE_RECONEXION_ACEPTADA:
				Logger::getLogger()->info("RECONEXION ACEPTADA");
				break;
			case EVENTO_CLIENTE_CONEXION_ACEPTADA:
				Logger::getLogger()->info("CONEXION ACEPTADA");
				break;
			/*
			case EVENTO_CLIENTE_CONEXION_RECHAZADA_NOMBRE_EXISTENTE:
				Logger::getLogger()->info("CONEXION RECHAZADA NOMBREEXISTENTE");
				break;
			case EVENTO_CLIENTE_CONEXION_RECHAZADA_SALA_LLENA:
				Logger::getLogger()->info("CONEXION RECHAZADA SALA LLENA");
				this->juegoSalir = true;
				break;
			*/
			//case EVENTO_CLIENTE_PERSONAJE_NUEVO_ID:
			//	this->personajeId = eventoServidor.id;
			//	break;
			case EVENTO_CLIENTE_FIGURA_ACTUALIZAR:
			case EVENTO_CLIENTE_PERSONAJE_CREADO: 
			case EVENTO_CLIENTE_PERSONAJE_QUIETO:
			case EVENTO_CLIENTE_PERSONAJE_QUIETO_IZQ:
			case EVENTO_CLIENTE_PERSONAJE_QUIETO_DER:
			case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_DER: 
			case EVENTO_CLIENTE_PERSONAJE_CAMINANDO_IZQ:

			case EVENTO_CLIENTE_PERSONAJE_SALTANDO: 
			case EVENTO_CLIENTE_PERSONAJE_SALTANDO_IZQ:
			case EVENTO_CLIENTE_PERSONAJE_SALTANDO_DER:

			case EVENTO_CLIENTE_FIGURA_MURIENDO:
			case EVENTO_CLIENTE_PERSONAJE_DESCONECTADO:
			case EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_1:
			case EVENTO_CLIENTE_ENEMIGO_CON_NIEVE_2:
			case EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE:

			case EVENTO_CLIENTE_ENEMIGO_BOLA_DE_NIEVE_PATEADA:

			case EVENTO_CLIENTE_NIEVE_IZQ:

			case EVENTO_CLIENTE_PROYECTIL_IZQ:
			case EVENTO_CLIENTE_PROYECTIL_DER:

			case EVENTO_CLIENTE_SNOWBOY_ATRAPADO_EN_BOLA_DE_NIEVE:
			case EVENTO_CLIENTE_SNOWBOY_INVENCIBLE:

			case EVENTO_CLIENTE_PERSONAJE_CORRIENDO_DER:
			case EVENTO_CLIENTE_PERSONAJE_CORRIENDO_IZQ:
			case EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_DER:
			case EVENTO_CLIENTE_PERSONAJE_CORRIENDO_QUIETO_IZQ:

			case EVENTO_CLIENTE_SNOWBOY_VA_A_RENACER:

			case EVENTO_CLIENTE_BONUS_HABILITADO:
			case EVENTO_CLIENTE_BONUS_DESHABILITADO:

				while(this->imprimiendoPantalla){
					this->actualizandoObjetoCV->sleep();
				}

				this->actualizandoObjeto = true;
				if( this->escenarioVista->existeObjeto( eventoServidor.id ) ){
					this->escenarioVista->getObjeto( eventoServidor.id )->setEstado( eventoServidor );
				}else{
					Vista* aux = this->crearObjetoVista(eventoServidor);
					if( aux != nullptr){
						this->escenarioVista->addObjeto( eventoServidor.id, aux );
					}
				}
				this->actualizandoObjeto = false;
				this->imprimiendoPantallaCV->wake();
				
				//this->escenarioVista->dibujar();
				break;
			case EVENTO_CLIENTE_SNOWBOY_PUNTAJE_Y_VIDAS:
				// En X van las vidas
				ControladorCliente::estadisticasJugadores->setVidas(eventoServidor.id, eventoServidor.x);
				// En Y van los puntos
				ControladorCliente::estadisticasJugadores->setPuntos(eventoServidor.id, eventoServidor.y);
				break;
			case EVENTO_CLIENTE_FIGURA_MUERTO:
				if( this->escenarioVista->existeObjeto( eventoServidor.id ) ){
                    Logger::getLogger()->debug("EVENTO_CLIENTE_FIGURA_MUERTO");
       		
                    while(this->imprimiendoPantalla){
    					this->actualizandoObjetoCV->sleep();
	    			}

		    		this->actualizandoObjeto = true;

					this->escenarioVista->elminarObjeto( eventoServidor.id );
       				this->actualizandoObjeto = false;
    				this->imprimiendoPantallaCV->wake();

				}
				break;
			case EVENTO_ESCENARIO_NIVEL_COMPLETADO:
				if(this->juegoFinalizado == false){
					// Muestra un cartel por sdl
					this->escenarioVista->statusMostrarEscenarioCompletado = true;
				}

				// Indica al socket que debe esperar por struct especial de escenario
				this->cliente->esperarNuevoEscenario = true;

				// Flag para saber cuando dejar de mostrar el cartel por sdl
				this->esperandoNuevoEscenario = true;

				this->juegoFinalizado = false;

				this->escenarioVista->statusMostrarEscenarioGameOver = false;
				this->escenarioVista->statusMostrarJuegoCompletado = false;
				break;
			case EVENTO_ESCENARIO_GAME_OVER:
				// Muestra un cartel por sdl
				this->escenarioVista->statusMostrarEscenarioGameOver = true;
				this->escenarioVista->statusMostrarJuegoCompletado = false;
				this->escenarioVista->statusMostrarEscenarioCompletado = false;
				// Indica al controlador que el juego finalizo, y espera un posible reinicio o exit
				this->juegoFinalizado = true;
				this->resetEnviado = false;

				// Indica al socket que debe esperar por struct especial de escenario
				this->cliente->esperarNuevoEscenario = true;

				// Flag para saber cuando dejar de mostrar el cartel por sdl
				this->esperandoNuevoEscenario = true;

				this->escenarioVista->statusMostrarCartelTeclas = true;
				break;
			case EVENTO_ESCENARIO_JUEGO_COMPLETADO:
				// Muestra un cartel por sdl
				this->escenarioVista->statusMostrarJuegoCompletado = true;
				this->escenarioVista->statusMostrarEscenarioGameOver = false;
				this->escenarioVista->statusMostrarEscenarioCompletado = false;

				// Indica al controlador que el juego finalizo, y espera un posible reinicio o exit
				this->juegoFinalizado = true;
				this->resetEnviado = false;

				// Indica al socket que debe esperar por struct especial de escenario
				this->cliente->esperarNuevoEscenario = true;

				// Flag para saber cuando dejar de mostrar el cartel por sdl
				this->esperandoNuevoEscenario = true;

				this->escenarioVista->statusMostrarCartelTeclas = true;
				break;
			case EVENTO_CLIENTE_JUEGO_STEP:
				//this->escenarioVista->dibujar();
				if(this->firstStep == false){	
					this->imprimiendoPantallaCV->wake();
					this->firstStep = true;
				}
				break;
		}
		Sleep(1);
    }
}

void ControladorCliente::imprimirPantalla(){

	while(this->actualizandoObjeto == true || this->firstStep == false){
		this->imprimiendoPantallaCV->sleep();
	}

	this->imprimiendoPantalla = true;
	this->escenarioVista->dibujar();
	this->imprimiendoPantalla = false;

	this->actualizandoObjetoCV->wake();

	SDL_Delay(1000/29);
		

}


void ControladorCliente::obtenerEventoPantalla(){
	sEventoCliente eventoCliente = {-1, EVENTO_SERVIDOR_NULL};
	//while(eventoCliente.evento != EVENTO_SERVIDOR_SALIR){
	while(!this->juegoSalir){
		Sleep(1);

		eventoCliente.id = -1;
		eventoCliente.evento = EVENTO_SERVIDOR_NULL;
		SDL_Event evento;

		
		if(SDL_WaitEvent(&evento)){
			if (evento.type == SDL_QUIT){

				eventoCliente.id     = Utilities::ID_SNOWBOY;
				eventoCliente.evento = EVENTO_SERVIDOR_SALIR;
				this->cliente->enqueueEventoEnviar( eventoCliente );
				Sleep(30);
				this->juegoSalir = true;

			}else if (evento.type == SDL_KEYDOWN){

				switch(evento.key.keysym.sym){
					case SDLK_q:
						eventoCliente.id     = Utilities::ID_SNOWBOY;
						eventoCliente.evento = EVENTO_SERVIDOR_SALIR;
						this->cliente->enqueueEventoEnviar( eventoCliente );
						Sleep(30);
						this->juegoSalir = true;
						break;

					//case SDLK_p:
					//	eventoCliente.evento = EVENTO_SERVIDOR_PAUSA;
					//	this->cliente->enqueueEventoEnviar( eventoCliente );
					//	break;	

					case SDLK_r:
						if(this->juegoFinalizado && this->resetEnviado == false){
							this->resetEnviado = true;

							eventoCliente.evento = EVENTO_SERVIDOR_RESET;
							this->cliente->enqueueEventoEnviar( eventoCliente );

							this->escenarioVista->statusMostrarCartelTeclas = false;
						}
						break;

					case SDLK_LEFT:
						if(!this->KeyPress_left){
							this->KeyPress_left = true;
							//eventoCliente.id = this->personajeId;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
						break;

					case SDLK_RIGHT:
						if(!this->KeyPress_right){
							this->KeyPress_right = true;
							//eventoCliente.id = this->personajeId;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_MOVER_DER_PERSONAJE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
						break;

					case SDLK_UP:
						if(!this->KeyPress_up){
							this->KeyPress_up = true;
							//eventoCliente.id = this->personajeId;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_SALTAR_PERSONAJE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
						break;
					case SDLK_PLUS:
					case SDLK_z:{
						    this->escenarioVista->dibujarZoomIn();
							//if (cambioElZoom){this->enqueueEventoZoom();}
        					break;
						}
					case SDLK_MINUS:
					case SDLK_x:{
							this->escenarioVista->dibujarZoomOut();
							//if (cambioElZoom){this->enqueueEventoZoom();}
							break;
						}
					case SDLK_m:{
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_MATAR_FIGURA;
							this->cliente->enqueueEventoEnviar( eventoCliente );
							break;
						}
					case SDLK_SPACE:
					case SDLK_d:
						if(!this->KeyPress_space){
							this->KeyPress_space = true;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_SNOWBOY_LANZAR_NIEVE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
				}
			}else if (evento.type == SDL_KEYUP){
				switch(evento.key.keysym.sym){
					case SDLK_LEFT:
						if(this->KeyPress_left){
							this->KeyPress_left = false;
							//eventoCliente.id = this->personajeId;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
						break;

					case SDLK_RIGHT:
						if(this->KeyPress_right){
							this->KeyPress_right = false;
							//eventoCliente.id = this->personajeId;
							eventoCliente.id = Utilities::ID_SNOWBOY;
							eventoCliente.evento = EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE;
							this->cliente->enqueueEventoEnviar( eventoCliente );
						}
						break;
					case SDLK_UP:
						if(this->KeyPress_up){
							this->KeyPress_up = false;
						}
						break;
					case SDLK_SPACE:
					case SDLK_d:
						if(this->KeyPress_space){
							this->KeyPress_space = false;
						}
						break;
				}
			}
		}
	}
}


Vista* ControladorCliente::crearObjetoVista(sEvento eventoServidor){
	switch(eventoServidor.tipo){
		case TIPO_CIRCULO:
			return new CirculoVista(eventoServidor);
			break;
		case TIPO_RECTANGULO:
			return new RectanguloVista(eventoServidor);
			break;
		case TIPO_PARALELOGRAMO:
			return new ParalelogramoVista(eventoServidor);
			break;
		case TIPO_TRAPECIO:
			return new TrapecioVista(eventoServidor);
			break;
		case TIPO_TRIANGULO:
			return new TrianguloVista(eventoServidor);
			break;
		case TIPO_PENTAGONO:
			return new PentagonoVista(eventoServidor);
			break;
		case TIPO_HEXAGONO:
			return new HexagonoVista(eventoServidor);
			break;
		case TIPO_CUADRADO:
			return new CuadradoVista(eventoServidor);
			break;
   		case TIPO_SNOWBOY:
            //this->personajeId = eventoServidor.id;
			return new SnowBoyVista(eventoServidor);
			//return new EnemigoVista(eventoServidor);
			break;
		case TIPO_DIABLO:
			return new DiabloVista(eventoServidor);
			break;
		case TIPO_NIEVE:
			return new NieveVista(eventoServidor);
			break;
		case TIPO_DRAGON:
			return new DragonVista(eventoServidor);
			break;
		case TIPO_FUEGO:
			return new FuegoVista(eventoServidor);
			break;
		case TIPO_BONUS_VELOCIDAD:
			return new BonusVelocidadVista(eventoServidor);
			break;
		case TIPO_BONUS_VIDA:
			return new BonusVidaVista(eventoServidor);

	}
	return nullptr;
}


DWORD WINAPI capturarEventos( LPVOID p ){
	// Agrega nuevos clientes al servidor
	ControladorCliente* controlador = (ControladorCliente*)p;

	//while(true){
	//	Sleep(20);
 //       controlador->obtenerEventoPantalla();
	//}

	controlador->procesarEventos();
	return 0;
}

DWORD WINAPI actualizarPantalla( LPVOID p ){
	//Sleep(1000);
	ControladorCliente* controlador = (ControladorCliente*)p;
	while(!controlador->juegoSalir){
		controlador->imprimirPantalla();
		Sleep(1);
	}
	return 0;
}

void ControladorCliente::enqueueEventoZoom(){
	sEvento eventoZoom;
	eventoZoom.evento = EVENTO_CLIENTE_JUEGO_STEP;
	this->cliente->enqueueEventoRecibido(eventoZoom);
}