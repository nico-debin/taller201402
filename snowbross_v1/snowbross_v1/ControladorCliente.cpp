#include "stdafx.h"
#include "ControladorCliente.h"
#include "CirculoVista.h"
#include "RectanguloVista.h"
#include "ParalelogramoVista.h"
#include "TrianguloVista.h"
#include "CuadradoVista.h"
#include "PentagonoVista.h"
#include "HexagonoVista.h"
#include "TrapecioVista.h"
#include "SnowBoyVista.h"

ControladorCliente::ControladorCliente(){
	this->escenarioVista = new EscenarioVista();
	//this->juegoEnPausa = false;
	Utilities::PAUSA_CLIENTE = false;
}

ControladorCliente::ControladorCliente( EscenarioVista* escenarioVista){
    this->setEscenarioVista(escenarioVista);
	//this->juegoEnPausa = false;
	Utilities::PAUSA_CLIENTE = false;
}

ControladorCliente::~ControladorCliente(){
	delete this->escenarioVista;
}

void ControladorCliente::setEscenarioVista(EscenarioVista* escenarioVista){
	this->escenarioVista = escenarioVista;
}

bool ControladorCliente::procesar(SDL_Event evento){
	bool quit = false;
	
	//SDL_Event evento
	// EscenarioVista.actualizar(vectorEventosServidor);
	// EscenarioVista.dibujar();
	// sEventoCliente* eventoCliente;
	// if(SDL_PollEvent(&evento)){
	//		eventoCliente = armarEventoCliente(evento);
    //   }else{
	//	   eventoCliente = nullptr;
    //         }
	//   return eventoCliente;


	// Obtengo el personaje
	Vista* snowBoyVista = this->escenarioVista->getObjeto(this->personajeId);

	sEvento nuevoEstadoPersonaje;		
	//nuevoEstadoPersonaje.evento = EVENTO_ACTUALIZAR_FIGURA;
	nuevoEstadoPersonaje.evento = EVENTO_CLIENTE_FIGURA_ACTUALIZAR;
	nuevoEstadoPersonaje.id     = snowBoyVista->getId();
	nuevoEstadoPersonaje.x      = snowBoyVista->get_x();
	nuevoEstadoPersonaje.y      = snowBoyVista->get_y();
	nuevoEstadoPersonaje.rot    = snowBoyVista->get_rot();
	nuevoEstadoPersonaje.alto   = snowBoyVista->get_alto();
	nuevoEstadoPersonaje.ancho  = snowBoyVista->get_ancho();
	nuevoEstadoPersonaje.color  = "#FF0000"; // notacion #rrggbb

	if (evento.type == SDL_QUIT){
		quit = true;

	}else if (evento.type == SDL_KEYDOWN){

		// Obtengo el circulo
		//Vista* circulo = this->escenarioVista->getObjeto(2);

		//Actualizacion del circulo
		//sEvento nuevoEstadoCirculo;
		/*
		nuevoEstadoCirculo.evento = EVENTO_ACTUALIZAR_FIGURA;
		nuevoEstadoCirculo.id     = circulo->getId();
		nuevoEstadoCirculo.x      = circulo->get_x();
		nuevoEstadoCirculo.y      = circulo->get_y();
		nuevoEstadoCirculo.rot    = circulo->get_rot();
		nuevoEstadoCirculo.alto   = circulo->get_alto();
		nuevoEstadoCirculo.ancho  = circulo->get_ancho();
		nuevoEstadoCirculo.color  = "#FF0000"; // notacion #rrggbb
		*/				

		switch(evento.key.keysym.sym){
			case SDLK_q:
				quit = true;
				break;
			
			case SDLK_p:
				system("pause");
				break;

			case SDLK_UP:
				//nuevoEstadoCirculo.y -= 10;
				//nuevoEstadoCirculo.color  = "#FF00FF"; // notacion #rrggbb
				nuevoEstadoPersonaje.y -= 10;
				break;
			case SDLK_DOWN:
				//nuevoEstadoCirculo.y += 10;
				//nuevoEstadoCirculo.color  = "#00FF00"; // notacion #rrggbb
				nuevoEstadoPersonaje.y += 10;
				break;
			case SDLK_RIGHT:
				//nuevoEstadoCirculo.x += 10;
				//nuevoEstadoCirculo.color  = "#F0F0FF"; // notacion #rrggbb
				nuevoEstadoPersonaje.x += 10;
				break;
			case SDLK_LEFT:
				//nuevoEstadoCirculo.x -= 10;
				//nuevoEstadoCirculo.color  = "#FFFF00"; // notacion #rrggbb
				nuevoEstadoPersonaje.x -= 10;
				break;
			case SDLK_m:
				//nuevoEstadoCirculo.x -= 10;
				//nuevoEstadoCirculo.color  = "#FFFF00"; // notacion #rrggbb
				//((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(MURIENDO);
				((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(EVENTO_CLIENTE_PERSONAJE_MURIENDO);
				break;
			case SDLK_c:
				//nuevoEstadoCirculo.x -= 10;
				//nuevoEstadoCirculo.color  = "#FFFF00"; // notacion #rrggbb
				//((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(CREADO);
				((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(EVENTO_CLIENTE_PERSONAJE_CREADO);
				break;
			case SDLK_v:
				//nuevoEstadoCirculo.x -= 10;
				//nuevoEstadoCirculo.color  = "#FFFF00"; // notacion #rrggbb
				//((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(QUIETO);
				((SnowBoyVista*)snowBoyVista)->setEstadoPersonaje(EVENTO_CLIENTE_PERSONAJE_QUIETO);
				break;

		}

		//circulo->setEstado(nuevoEstadoCirculo);
		//((SnowBoyVista*)snowBoyVista)->setEstado(nuevoEstadoPersonaje);
	}
	((SnowBoyVista*)snowBoyVista)->setEstado(nuevoEstadoPersonaje);
	this->escenarioVista->dibujar();
	return quit;
}

sEventoCliente ControladorCliente::procesarEventos(vector<sEvento*>* vectorEventosServidor, Escenario* escenario){
	sEventoCliente eventoCliente = {-1, EVENTO_SERVIDOR_NULL};

	// Actualizo las vistas de cada figura
	for(std::vector<sEvento*>::iterator it = vectorEventosServidor->begin(); it != vectorEventosServidor->end(); it++){
		sEvento* eventoServidor = (*it);

		switch(eventoServidor->evento){
			case EVENTO_CLIENTE_FIGURA_ACTUALIZAR:
			case EVENTO_CLIENTE_PERSONAJE_CREADO: 
			case EVENTO_CLIENTE_PERSONAJE_QUIETO:
			case EVENTO_CLIENTE_PERSONAJE_CAMINANDO: 
			case EVENTO_CLIENTE_PERSONAJE_SALTANDO: 
			case EVENTO_CLIENTE_PERSONAJE_MURIENDO:
			case EVENTO_CLIENTE_PERSONAJE_MUERTO:
				if( this->escenarioVista->existeObjeto( eventoServidor->id ) ){
					this->escenarioVista->getObjeto( eventoServidor->id )->setEstado( *eventoServidor );
				}else{
					this->escenarioVista->addObjeto( eventoServidor->id, this->crearObjetoVista(eventoServidor) );
				}
				break;
			case EVENTO_CLIENTE_JUEGO_RESET:
				delete this->escenarioVista;
				this->escenarioVista = new EscenarioVista();
				return eventoCliente;
			case EVENTO_CLIENTE_JUEGO_PAUSA:
				Utilities::PAUSA_CLIENTE = !Utilities::PAUSA_CLIENTE;
				return eventoCliente;
		}

	}
	
	this->escenarioVista->mapaObjetoFisicos = *escenario->getFiguras();

	// Dibujo el escenario
	this->escenarioVista->dibujar();

	SDL_Event evento;
	if(SDL_PollEvent(&evento)){
		if (evento.type == SDL_QUIT){
			eventoCliente.evento = EVENTO_SERVIDOR_SALIR;
		}else if (evento.type == SDL_KEYDOWN){
			switch(evento.key.keysym.sym){
				case SDLK_q:
					eventoCliente.evento = EVENTO_SERVIDOR_SALIR;
					break;

				case SDLK_p:
					eventoCliente.evento = EVENTO_SERVIDOR_PAUSA;
					break;	

				case SDLK_r:
					eventoCliente.evento = EVENTO_SERVIDOR_RESET;
					break;

                case SDLK_LEFT:
                    eventoCliente.id = this->personajeId;
                    eventoCliente.evento = EVENTO_SERVIDOR_MOVER_IZQ_PERSONAJE;
                    break;

                case SDLK_RIGHT:
                    eventoCliente.id = this->personajeId;
                    eventoCliente.evento = EVENTO_SERVIDOR_MOVER_DER_PERSONAJE;
                    break;

                case SDLK_UP:
                    eventoCliente.id = this->personajeId;
                    eventoCliente.evento = EVENTO_SERVIDOR_SALTAR_PERSONAJE;
                    break;
				case SDLK_z:
					Utilities::zoom+=0.1;
					break;
				case SDLK_x:
					Utilities::zoom-=0.1;
					break;
				case SDLK_d:
					Utilities::debugDraw = !Utilities::debugDraw;		
					break;
			}
		}else if (evento.type == SDL_KEYUP){
			switch(evento.key.keysym.sym){
				case SDLK_LEFT:
					eventoCliente.id = this->personajeId;
					eventoCliente.evento = EVENTO_SERVIDOR_FRENAR_IZQ_PERSONAJE;
                    break;

                case SDLK_RIGHT:
                    eventoCliente.id = this->personajeId;
					eventoCliente.evento = EVENTO_SERVIDOR_FRENAR_DER_PERSONAJE;
                    break;
			}
		}
	}
	
	return eventoCliente;
}

Vista* ControladorCliente::crearObjetoVista(sEvento* eventoServidor){
	//if (eventoServidor->evento == EVENTO_CLIENTE_FIGURA_ACTUALIZAR){
		switch(eventoServidor->tipo){
			case TIPO_CIRCULO:
				return new CirculoVista(*eventoServidor);
				break;
			case TIPO_RECTANGULO:
				return new RectanguloVista(*eventoServidor);
				break;
			case TIPO_PARALELOGRAMO:
				return new ParalelogramoVista(*eventoServidor);
				break;
			case TIPO_TRAPECIO:
				return new TrapecioVista(*eventoServidor);
				break;
			case TIPO_TRIANGULO:
				return new TrianguloVista(*eventoServidor);
				break;
			case TIPO_PENTAGONO:
				return new PentagonoVista(*eventoServidor);
				break;
			case TIPO_HEXAGONO:
				return new HexagonoVista(*eventoServidor);
				break;
			case TIPO_CUADRADO:
				return new CuadradoVista(*eventoServidor);
				break;
   			case TIPO_SNOWBOY:
                this->personajeId = eventoServidor->id;
				return new SnowBoyVista(*eventoServidor);
				//return new CirculoVista(*eventoServidor);
				break;	
		//}
	}
	return nullptr;
}