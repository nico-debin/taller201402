#ifndef _SnowBoyVista_H_
#define _SnowBoyVista_H_

#include "PersonajeVista.h"
#include "EstructuraDeDatos.h"
#include "Timer.h"

class SnowBoyVista : public PersonajeVista {

	private:
			void inicializacionEstados();
			
			//La diferencia con estadoPersonaje es que al Real no se le pone el estado SNOWBOY_INVENCIBLE y evita los problemas de sonido cuando es invencible
			int estadoPersonajeReal;
			ControlSonido controlSonido;
			Timer timerDesconectado;
		 
	public:
			SnowBoyVista(sEvento estadoEvento);	
			void dibujar( SDL_Renderer* renderer );
			virtual void setEstado( sEvento estadoEvento );

};

#endif

