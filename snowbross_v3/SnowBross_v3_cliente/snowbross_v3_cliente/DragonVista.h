#ifndef _DragonVista_H_
#define _DragonVista_H_

#include "PersonajeVista.h"
#include "EstructuraDeDatos.h"

class DragonVista : public PersonajeVista {
	private:
		void inicializacionEstados();
	public:
		DragonVista(sEvento estadoEvento);
		void dibujar( SDL_Renderer* renderer );
};


#endif