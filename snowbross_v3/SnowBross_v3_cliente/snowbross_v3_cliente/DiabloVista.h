#ifndef _DiabloVista_H_
#define _DiabloVista_H_

#include "PersonajeVista.h"
#include "EstructuraDeDatos.h"

class DiabloVista : public PersonajeVista {
	private:
		void inicializacionEstados();
	public:
		DiabloVista(sEvento estadoEvento);
		void dibujar( SDL_Renderer* renderer );
};


#endif