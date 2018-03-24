#pragma once

#ifndef _JuegoControlador_H_
#define _JuegoControlador_H_

#include "Parser.h"
#include "estructuraDeDatos.h"
#include "ControladorCliente.h"
#include <windows.h>

class JuegoControlador{

public:
	Parser* parser;
	Escenario *escenario;
	ControladorCliente* controladorCliente;

	JuegoControlador(void);
	~JuegoControlador(void);

	void iniciar(string nombreArchivoConfiguracion);
	
private: 
	bool juegoEnPausa;

	void iniciarEscenario();
	void procesarEventoCliente( sEventoCliente );

	sEventoCliente enviarEventosACliente(vector<sEvento*>*);
};

#endif



