#ifndef _ControladorCliente_H_
#define _ControladorCliente_H_

#include "EscenarioVista.h"
#include "Utilities.h"
#include "Cliente.h"
#include "ConditionVariable.h"
#include "EstadisticasJugadores.h"

#include <SDL2/SDL.h>

class EscenarioVista; // Evita inclusión ciclica

class ControladorCliente{
	private:
		EscenarioVista* escenarioVista;
		//Vista* crearObjetoVista(sEvento* eventoServidor);
        Vista* crearObjetoVista(sEvento eventoServidor);
        
        Cliente* cliente;
        HANDLE capturarEventoThread;
        HANDLE actualizarPantallaThread;

		//Lock* lockActualizandoObjeto;
		bool imprimiendoPantalla;
		bool actualizandoObjeto;
		bool firstStep;
		bool esperandoNuevoEscenario;
		bool juegoFinalizado;
		bool resetEnviado;

		ConditionVariable* imprimiendoPantallaCV;
		ConditionVariable* actualizandoObjetoCV;
		
        bool semaforoPantalla;

		bool KeyPress_left;
		bool KeyPress_right;
		bool KeyPress_up;
		bool KeyPress_space;


		void enqueueEventoZoom();

		

	public:
		static EstadisticasJugadores* estadisticasJugadores;
		int personajeId;
		//static bool juegoEnPausa;
		
		bool juegoSalir;

		ControladorCliente( string username, string ip, int puerto );
		ControladorCliente( EscenarioVista* escenarioVista );
		~ControladorCliente();
				
		void iniciar();

		void setEscenarioVista(EscenarioVista* escenarioVista);

		void imprimirPantalla();

		// Devuelve true si se capturó el evento para salir del juego
		//bool procesar(SDL_Event evento);

		//sEventoCliente procesarEventos(vector<sEvento*>* vectorEventosServidor, Escenario* escenario);
        void procesarEventos();

        //sEventoCliente obtenerEventoPantalla();
        void obtenerEventoPantalla();
		
};

#endif
