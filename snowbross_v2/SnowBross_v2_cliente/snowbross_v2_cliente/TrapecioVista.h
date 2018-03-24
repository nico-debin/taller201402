#pragma once
#ifndef _TrapecioVista_H_
#define _TrapecioVista_H_

#include "vista.h"
class TrapecioVista : public Vista {
	private:
		short vertices_x[4], vertices_y[4];
		void calcularVertices();
	public:
		TrapecioVista(sEvento estadoEvento);
		void setEstado( sEvento estadoEvento );
		void setVertices(float* x, float* y);
		//void dibujar(SDL_Renderer* renderer);
};


#endif /* _TrapecioVista_H_ */