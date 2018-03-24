#ifndef _ControladorCliente_H_
#define _ControladorCliente_H_

#include "Escenario.h"
#include "EscenarioVista.h"
#include "Utilities.h"

#include <SDL2/SDL.h>

class ControladorCliente{
	private:
		EscenarioVista* escenarioVista;
		Vista* crearObjetoVista(sEvento* eventoServidor);
		
	public:
		int personajeId;
		//static bool juegoEnPausa;
		ControladorCliente();
		ControladorCliente( EscenarioVista* escenarioVista );
		~ControladorCliente();
				
		void setEscenarioVista(EscenarioVista* escenarioVista);

		// Devuelve true si se capturó el evento para salir del juego
		bool procesar(SDL_Event evento);

		sEventoCliente procesarEventos(vector<sEvento*>* vectorEventosServidor, Escenario* escenario);
};

#endif
