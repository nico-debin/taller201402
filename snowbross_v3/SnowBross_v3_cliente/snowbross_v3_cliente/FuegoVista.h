#ifndef _FuegoVista_H_
#define _FuegoVista_H_
#include "ProyectilVista.h"
#include "EstructuraDeDatos.h"

class FuegoVista : public ProyectilVista{
	protected:
		void iniciarClips();
	public:
		FuegoVista(sEvento estadoEvento);
};

#endif

