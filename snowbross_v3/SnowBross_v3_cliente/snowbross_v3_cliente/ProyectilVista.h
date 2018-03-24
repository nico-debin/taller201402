#ifndef _ProyectilVista_H_
#define _ProyectilVista_H_
#include "Vista.h"

class ProyectilVista: public Vista{

	protected:
		int estadoProyectil;
		
		SDL_Rect clipDer,clipIzq;

		virtual void iniciarClips()=0;

		SDL_Rect* elegirClip();

	public:
		ProyectilVista(sEvento estadoEvento);
		void dibujar(SDL_Renderer* renderer);
		void setEstado( sEvento estadoEvento );
		
};


#endif