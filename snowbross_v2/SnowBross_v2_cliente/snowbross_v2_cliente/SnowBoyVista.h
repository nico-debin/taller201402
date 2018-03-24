#pragma once
#ifndef _SnowBoyVista_H_
#define _SnowBoyVista_H_

#include "PersonajeVista.h"
#include "EstructuraDeDatos.h"

class SnowBoyVista : public PersonajeVista {

	private:
			void inicializacionEstados();
		 
	public:
			SnowBoyVista(sEvento estadoEvento);	

};

#endif

