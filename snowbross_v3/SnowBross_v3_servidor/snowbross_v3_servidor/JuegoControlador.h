#pragma once

#ifndef _JuegoControlador_H_
#define _JuegoControlador_H_

#include "Parser.h"
#include "estructuraDeDatos.h"
#include "Servidor.h"
#include <windows.h>
#include "Fuego.h"
#include <map>
#include "EstadisticasJugadores.h"
#include "ConditionVariable.h"

class SnowWorldContactListener;

class JuegoControlador{

public:
	Parser* parser;
	Escenario *escenario;	
    Servidor* servidor;    

	JuegoControlador(void);
	~JuegoControlador(void);

	void iniciar(string nombreArchivoConfiguracion, int puerto);
	void procesarEventoCliente();
	void encolarEventosEstadisticas();

	

	static EstadisticasJugadores* estadisticasJugadores;

	SnowWorldContactListener* contactListener;
	int nuevoJuegoQuorumNecesario;
	
private: 
	bool juegoEnPausa;
    bool juegoSalir;
    bool semaforo;
	bool juegoFinalizado;
	bool juegoAux;
	//int nuevoJuegoQuorumNecesario;
	int juegoFinalizado_cantClientesQueSeDesconectaron;

	ConditionVariable* esperarNuevoJuegoQuorum;

    HANDLE procesarEventoThread;

	void iniciarEscenario();    
    void procesarEscenario();
	void cambiarNivel();
	void cambiarNivelInicial();
	void procesarReinicioJuego();
};

#endif



