#ifndef _NieveVista_H_
#define _NieveVista_H_
#include "ProyectilVista.h"
#include "EstructuraDeDatos.h"
#include "ControlSonido.h"

class NieveVista : public ProyectilVista{
	protected:
		void iniciarClips();
		ControlSonido controlSonido;
	public:
		NieveVista(sEvento estadoEvento);
		~NieveVista();
};

#endif

