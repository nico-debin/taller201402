#pragma once

#ifndef _JuegoControlador_H_
#define _JuegoControlador_H_

#include "Parser.h"
#include "estructuraDeDatos.h"
#include "Servidor.h"
#include <windows.h>

class JuegoControlador{

public:
	Parser* parser;
	Escenario *escenario;	
    Servidor* servidor;    

	JuegoControlador(void);
	~JuegoControlador(void);

	void iniciar(string nombreArchivoConfiguracion, int puerto);
	void procesarEventoCliente();
	
private: 
	bool juegoEnPausa;
    bool juegoSalir;
    bool semaforo;

    HANDLE procesarEventoThread;

	void iniciarEscenario();    
    void procesarEscenario();
};

#endif



