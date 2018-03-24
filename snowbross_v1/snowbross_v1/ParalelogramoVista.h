#pragma once
#ifndef _ParalelogramoVista_H_
#define _ParalelogramoVista_H_

#include "vista.h"
class ParalelogramoVista : public Vista {
	private:
		short vertices_x[4], vertices_y[4];
		void calcularVertices();
	public:
		ParalelogramoVista(sEvento estadoEvento);
		void setEstado( sEvento estadoEvento );
		void setVertices(float* x, float* y);
		void dibujar(SDL_Renderer* renderer);
		void dibujarTextura(SDL_Renderer* renderer);
};


#endif /* _ParalelogramoVista_H_ */