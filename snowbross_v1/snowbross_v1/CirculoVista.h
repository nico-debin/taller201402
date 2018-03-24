#ifndef _CirculoVista_H_
#define _CirculoVista_H_

#include "Vista.h"

class CirculoVista : public Vista {

	public:
		CirculoVista(sEvento estadoEvento);
		double degrees;
		void dibujar(SDL_Renderer* renderer);
		void dibujarTextura(SDL_Renderer* renderer);
};


#endif